/*
 * A045184  Numbers whose base-5 representation contains exactly three 0's and
 *          no 2's.
 * https://oeis.org/A045184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A045184:
   Numbers whose base‑5 representation contains exactly three 0's
   and no 2's.  It finds the n‑th such number by determining
   its base‑5 length and constructing the digits lexicographically. */
#include <stdint.h>
#include <stdbool.h>

/* ---------- small helper functions ----------------------------------- */
static inline seq_t pow5(unsigned int e)
{
    seq_t r = 1;
    while (e--) r *= 5;
    return r;
}

static inline seq_t pow3(unsigned int e)
{
    seq_t r = 1;
    while (e--) r *= 3;
    return r;
}

/* Binomial coefficient C(n,k) for small n (n <= 64).  The result fits
   into seq_t because the caller ensures n is small. */
static inline seq_t binom(unsigned int n, unsigned int k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Count of length‑L numbers (L ≥ 4) with exactly three 0's and no 2's. */
static inline seq_t count_len(unsigned int L)
{
    if (L < 4) return 0;
    /* total = 3 * C(L-1,3) * 3^(L-4) */
    return (seq_t)3 * binom(L - 1, 3) * pow3(L - 4);
}

/* Count per leading digit (1,3,4) */
static inline seq_t count_per_leading(unsigned int L)
{
    /* for a fixed leading digit: C(L-1,3) * 3^(L-4) */
    return binom(L - 1, 3) * pow3(L - 4);
}

/* ---------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* work in seq_t to avoid overflow */
    unsigned int L = 4;

    /* Find length L such that N lies within the block of length L. */
    while (true) {
        seq_t cnt = count_len(L);
        if (N <= cnt) break;
        N -= cnt;
        ++L;
    }

    /* Now N is 1‑based index within the length‑L block. */
    seq_t remaining = N;
    const unsigned char leading_digits[3] = {1, 3, 4};
    seq_t lead = leading_digits[0];
    seq_t cnt_per_lead = count_per_leading(L);

    /* Select the leading digit. */
    unsigned int lead_idx = 0;
    while (remaining > cnt_per_lead) {
        remaining -= cnt_per_lead;
        ++lead_idx;
    }
    lead = leading_digits[lead_idx];

    /* Start building the decimal value. */
    seq_t value = lead * pow5(L - 1);

    /* Remaining digits: positions 1 .. L-1.  We still need 3 zeros. */
    unsigned int zeros_needed = 3;
    const unsigned char digit_opts[4] = {0, 1, 3, 4};

    for (unsigned int pos = 1; pos < L; ++pos) {
        unsigned int rem_pos = L - pos;   /* positions left after current */

        /* Try each digit in ascending order. */
        for (unsigned int d_idx = 0; d_idx < 4; ++d_idx) {
            unsigned int digit = digit_opts[d_idx];

            if (digit == 0 && zeros_needed == 0) continue;   /* cannot place more zeros */

            unsigned int zeros_left = zeros_needed;
            if (digit == 0) --zeros_left;

            /* Ways to fill the rest after choosing this digit. */
            seq_t ways = binom(rem_pos - 1, zeros_left) * pow3((rem_pos - 1) - zeros_left);

            if (remaining > ways) {
                remaining -= ways;
                continue;
            }

            /* This digit is chosen. */
            value += (seq_t)digit * pow5(rem_pos - 1);
            zeros_needed = zeros_left;
            break;
        }
    }

    return value;
}
