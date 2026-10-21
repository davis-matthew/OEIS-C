/*
 * A079070  Number of numbers < n having in binary representation the same
 *          number of 0's as n.
 * https://oeis.org/A079070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A079070 – count of numbers < n having in binary representation the
    same number of 0 s as n.
    Formula: a(n) = #{ m : 1 ≤ m < n  and  zeros(m) = zeros(n) }.
    The routine works for all n ≥ 1 and uses O(64) pre‑computed binomial
    coefficients.  It is written in pure C11 and is compatible with
    arbitrary‐precision seq_t types. */
#include <stdint.h>

#define MAXBIT 64          /* maximum bit length of idx_t (uint64_t) */

/* binomial coefficients C[n][k] 0 ≤ n,k ≤ 64 */
static seq_t C[MAXBIT + 1][MAXBIT + 1] = {0};
static int binom_inited = 0;

static void init_binom(void)
{
    if (binom_inited) return;
    binom_inited = 1;
    for (int n = 0; n <= MAXBIT; ++n) {
        C[n][0] = C[n][n] = (seq_t)1;
        for (int k = 1; k < n; ++k)
            C[n][k] = C[n-1][k-1] + C[n-1][k];
    }
}

/*  Count numbers  m < bound  with exactly target_pop 1‑bits
    (so zeros = bitlen(bound) – target_pop).
    Numbers are assumed to have the same bit length as bound
    (i.e. the most significant bit of bound is 1). */
static seq_t count_less_same_length(idx_t bound, int target_pop)
{
    seq_t count = 0;

    /* if target_pop is impossible, zero */
    if (target_pop < 1) return count;
    int bits = 64 - __builtin_clzll(bound);   /* bit length of bound */
    int ones_needed = target_pop;             /* ones still to place */

    /* consume the leading 1 of the fixed-length numbers */
    --ones_needed;
    if (ones_needed < 0) return count;        /* cannot match */

    /* iterate over the remaining bits from MSB-1 down to 0 */
    for (int i = bits - 2; i >= 0; --i) {
        if ((bound >> i) & 1ULL) {            /* bound has 1 at this bit */
            /* set this bit to 0 → we get all completions with remaining ones */
            if (ones_needed <= i)
                count += C[i][ones_needed];
            /* keep this bit 1 to stay ≤ bound */
            --ones_needed;
            if (ones_needed < 0) break;
        }
        /* bound bit is 0 → candidate must also have 0 here */
    }

    return count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_binom();

    if (n <= 1) return 0;     /* a(1) = 0, and no numbers < 1 */

    int bits = 64 - __builtin_clzll(n);              /* bit length of n */
    int pop_n   = __builtin_popcountll(n);           /* popcount of n */
    int zeros_n = bits - pop_n;                      /* number of zeros in n */

    /* 1) Count numbers with fewer bits that have the same number of zeros. */
    seq_t total = 0;
    for (int l = 1; l < bits; ++l) {
        int target_pop = l - zeros_n;               /* required popcount for length l */
        if (target_pop < 1 || target_pop > l) continue;
        total += C[l-1][target_pop-1];
    }

    /* 2) Count numbers of the same length that are smaller than n. */
    int target_pop_same = bits - zeros_n;           /* popcount that matches zeros_n */
    total += count_less_same_length(n, target_pop_same);

    return total;
}
