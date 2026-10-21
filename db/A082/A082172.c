/*
 * A082172  A subclass of quasi-acyclic automata with 3 inputs, n transient
 *          and k absorbing labeled states.
 * https://oeis.org/A082172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(n), where T is read from the table S_3(n,k) by
 *            antidiagonals. S_3(n,k) satisfies: S_3(0,k) = 1 S_3(n,k) =
 *            Σ_{i=0}^{n-1} (-1)^(n-i-1) * binomial(n,i) * ((i+k+1)^3-1)^(n-i)
 *            * S_3(i,k) for n>0 This implementation memoises S_3 in a
 *            flattened 2‑D array. The table is expanded when needed and a...
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 19, u64 n <= 43, u128 n <= 103,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

static seq_t *S_table = NULL;          /* flattened 2‑D table S_3[n][k] */
static idx_t S_cap_n = 0;              /* largest stored n index (inclusive) */
static idx_t S_cap_k = 0;              /* largest stored k index (inclusive) */

/* Ensure S_table can store indices up to (n,k) inclusive.  */
static void ensure_S_capacity(idx_t n, idx_t k)
{
    size_t new_cap_n = S_cap_n;
    size_t new_cap_k = S_cap_k;

    if ((size_t)n > new_cap_n) new_cap_n = (size_t)n;
    if ((size_t)k > new_cap_k) new_cap_k = (size_t)k;

    /* Allocate the new table if bigger than current. */
    if (new_cap_n > S_cap_n || new_cap_k > S_cap_k)
    {
        size_t new_rows = new_cap_n + 1;   /* include index 0 */
        size_t new_cols = new_cap_k + 1;
        size_t new_size = new_rows * new_cols;

        seq_t *new_table = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_table) return;           /* allocation failure → leave old table */

        /* initialise all entries to zero */
        memset(new_table, 0, new_size * sizeof(seq_t));

        /* copy old data */
        if (S_table)
        {
            size_t old_cols = S_cap_k + 1;
            for (idx_t i = 0; i <= S_cap_n; ++i)
                for (idx_t j = 0; j <= S_cap_k; ++j)
                    new_table[i * new_cols + j] =
                        S_table[i * old_cols + j];
            free(S_table);
        }

        S_table = new_table;
        S_cap_n = (idx_t)new_cap_n;
        S_cap_k = (idx_t)new_cap_k;
    }
}

/* Recursive memoised computation of S_3(n,k). */
static seq_t S_3_value(idx_t n, idx_t k)
{
    if (n == 0) return 1;

    ensure_S_capacity(n, k);

    size_t width = S_cap_k + 1;
    size_t pos   = (size_t)n * width + (size_t)k;
    seq_t cached = S_table[pos];
    if (cached != 0) return cached;   /* already computed */

    seq_t positive = 0;
    seq_t negative = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t bin = binomial(n, i);

        seq_t base = (seq_t)i + (seq_t)k + 1;
        seq_t cube = base * base * base;
        seq_t pow_val = cube - 1;

        seq_t power = big_pow(pow_val, (seq_t)(n - i));
        seq_t term  = bin * power * S_3_value(i, k);

        if (((n - i - 1) & 1ULL) != 0)   /* sign = -1 */
            negative += term;
        else
            positive += term;
    }

    seq_t result = (positive >= negative) ? (positive - negative) : 0;
    S_table[pos] = result;
    return result;
}

/* Main sequence function.  a(n) is read by antidiagonals. */
seq_t a(idx_t n)
{
    /* Find s such that T_{s-1} <= n < T_s, where T_t = t*(t+1)/2. */
    idx_t low  = 1;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;        /* exponential search to upper bound */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mid_tri =
            ((unsigned __int128)mid * (mid + 1)) / 2;
        if (mid_tri > n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t s = low;
    unsigned __int128 prev_tri = ((unsigned __int128)(s - 1) * s) / 2;
    idx_t offset = (idx_t)(n - prev_tri);   /* i coordinate */
    idx_t k = s - offset;                   /* j coordinate */
    return S_3_value(offset, k);
}
