/*
 * A045041  Numbers whose base-4 representation contains no 0's and exactly
 *          four 2's.
 * https://oeis.org/A045041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1471, u32 n <= 4300, u64 n <= 4300,
 *            u128 n <= 4300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A045041:
   Numbers whose base‑4 representation contains no 0's and exactly four 2's.
   The term a(n) is returned for any n ≥ 1, regardless of call order.
   The implementation uses combinatorial enumeration of the base‑4 digits. */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) in seq_t.  k is at most 4 in this problem. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (n - k + i);
        result = result / i;           /* exact division */
    }
    return result;
}

/* Count how many length‑L numbers satisfy the conditions. */
static seq_t count_len(idx_t L)
{
    if (L < 4) return (seq_t)0;
    seq_t comb = binom_seq(L, 4);          /* choose positions of the 4 twos */
    seq_t pow2 = ((seq_t)1) << (L - 4);    /* remaining digits are 1 or 3 */
    return comb * pow2;
}

/* Main function: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n == 0, return 0 */

    seq_t target = (seq_t)(n - 1);                 /* 0‑based index */
    idx_t L = 4;                                 /* current length */
    seq_t cum = (seq_t)0;                        /* cumulative count so far */

    /* Find the length L that contains the target. */
    while (1) {
        seq_t cnt = count_len(L);
        if (cum + cnt > target) break;
        cum += cnt;
        ++L;
    }

    seq_t rank = target - cum;                   /* local rank within length L */

    /* Build the number digit by digit (most significant first). */
    seq_t value = (seq_t)0;
    idx_t remaining_twos = 4;
    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 1; d <= 3; ++d) {           /* try digits 1, 2, 3 in order */
            if (d == 2 && remaining_twos == 0) continue;
            idx_t new_twos = remaining_twos - ((d == 2) ? 1 : 0);
            idx_t positions_left = L - pos - 1;
            seq_t cnt_prefix = (seq_t)0;
            if (new_twos <= positions_left) {
                seq_t comb = binom_seq(positions_left, new_twos);
                seq_t pow2  = ((seq_t)1) << (positions_left - new_twos);
                cnt_prefix = comb * pow2;
            }
            if (rank >= cnt_prefix) {
                rank -= cnt_prefix;
                continue;
            }
            /* Selected digit */
            value = (value << 2) + (seq_t)d;   /* value*4 + d */
            remaining_twos = new_twos;
            break;
        }
    }

    return value;
}
