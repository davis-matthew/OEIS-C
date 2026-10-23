/*
 * A045179  Numbers whose base-5 representation contains exactly one 0 and
 *          three 2's.
 * https://oeis.org/A045179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation contains exactly one 0 and three 2’s,
   sorted in increasing numeric order.  The function generates the n‑th
   element directly by combinatorial enumeration.  */
#include <stdbool.h>

/* Compute binomial coefficient C(n,k) for small n,k (0 ≤ k ≤ n). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* use 128‑bit intermediate for safety on 64‑bit builds */
        __int128 tmp = (__int128)r * (n - k + i);
        tmp /= i;
        r = (seq_t)tmp;
    }
    return r;
}

/* Compute 3^exp for a small non‑negative exponent. */
static seq_t pow3(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 3;
    return r;
}

/* Count of length‑len strings with the given remaining zeros and twos.
   The first digit has no special restriction at this point. */
static seq_t count_rem(idx_t len, idx_t zeros_rem, idx_t twos_rem)
{
    if (zeros_rem > len) return 0;
    if (twos_rem > len - zeros_rem) return 0;
    seq_t zeroComb  = binom_small(len, zeros_rem);
    seq_t twosComb  = binom_small(len - zeros_rem, twos_rem);
    idx_t others    = len - zeros_rem - twos_rem;   /* positions for 1,3,4 */
    seq_t powTerm   = pow3(others);
    return zeroComb * twosComb * powTerm;
}

/* Number of k‑digit numbers (k ≥ 4) with exactly one 0 and three 2’s,
   first digit non‑zero. */
static seq_t count_k(idx_t k)
{
    if (k < 4) return 0;
    seq_t firstPart = (seq_t)(k - 1);          /* choose position of the zero (not first) */
    seq_t twosPart  = binom_small(k - 1, 3);   /* choose positions of the three twos among the remaining k‑1 places */
    seq_t others    = pow3(k - 4);             /* remaining positions can be 1,3,4 */
    return firstPart * twosPart * others;
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined by OEIS, but harmless */

    /* 1. Find the length k that contains the n‑th number. */
    idx_t   k         = 4;
    seq_t   cumulative = 0;

    for (;;) {
        seq_t cnt = count_k(k);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++k;
    }

    /* 2. Position within the block of length k. */
    seq_t offset = n - cumulative;          /* 1‑based index inside the block */

    /* 3. Construct the number digit by digit. */
    seq_t result    = 0;
    idx_t zeros_rem = 1;   /* need exactly one zero */
    idx_t twos_rem  = 3;   /* need exactly three twos */

    for (idx_t pos = 0; pos < k; ++pos) {
        for (int d = 0; d <= 4; ++d) {
            /* leading digit cannot be zero */
            if (pos == 0 && d == 0) continue;
            /* respect remaining zero / two counts */
            if (d == 0 && zeros_rem == 0) continue;
            if (d == 2 && twos_rem  == 0) continue;

            idx_t zeros_new = zeros_rem - (d == 0 ? 1 : 0);
            idx_t twos_new  = twos_rem  - (d == 2 ? 1 : 0);
            idx_t len_rem   = k - pos - 1;

            seq_t cnt = count_rem(len_rem, zeros_new, twos_new);
            if (cnt >= offset) {             /* this digit is chosen */
                result = result * 5 + d;
                zeros_rem = zeros_new;
                twos_rem  = twos_new;
                break;
            } else {                          /* skip this block */
                offset -= cnt;
            }
        }
    }

    return result;
}
