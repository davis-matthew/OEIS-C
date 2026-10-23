/*
 * A023722  Numbers with a single 0 in their base 5 expansion.
 * https://oeis.org/A023722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base‑5 expansion
 *            contains exactly one digit '0'
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static idx_t count_remaining(idx_t rem, int zero_needed, const seq_t *pow4) {
    if (zero_needed == 0) {
        return (idx_t)pow4[rem];
    } else {
        if (rem == 0) return 0;
        return (idx_t)(rem * pow4[rem - 1]);
    }
}

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)0;
    }

    /* index after the leading zero (0) */
    idx_t idx = n - 2;              /* 0‑based index among the numbers with L ≥ 2 */
    idx_t cumulative = 0;
    unsigned L = 2;

    /* find the required length L */
    for (L = 2; ; ++L) {
        seq_t pow = 1;
        for (unsigned i = 0; i < L - 1; ++i) pow *= (seq_t)4;
        seq_t count_len_seq = (seq_t)(L - 1) * pow;
        idx_t count_len = (idx_t)count_len_seq;

        if (cumulative + count_len > idx) {
            break;
        }
        cumulative += count_len;
    }

    idx_t pos = idx - cumulative;   /* 0‑based index within length L */

    /* precompute powers of 4 up to L */
    static const unsigned MAX_L = 64;   /* sufficient for n up to many thousands */
    seq_t pow4[MAX_L + 1];
    pow4[0] = (seq_t)1;
    for (unsigned i = 1; i <= L; ++i) {
        pow4[i] = pow4[i - 1] * (seq_t)4;
    }

    seq_t number = (seq_t)0;
    int zero_needed = 1;          /* whether a 0 still has to appear */

    for (unsigned pos_idx = 0; pos_idx < L; ++pos_idx) {
        unsigned remaining = L - pos_idx - 1;
        for (int d = 0; d <= 4; ++d) {
            /* leading zero is not allowed */
            if (pos_idx == 0 && d == 0) continue;
            /* if we have already used the zero, d cannot be zero */
            if (zero_needed == 0 && d == 0) continue;

            int zero_next = (zero_needed && d == 0) ? 0 : zero_needed;
            idx_t cnt = count_remaining(remaining, zero_next, pow4);

            if (pos < cnt) {
                number = number * (seq_t)5 + (seq_t)d;
                zero_needed = zero_next;
                break;
            } else {
                pos -= cnt;
            }
        }
    }

    return number;
}
