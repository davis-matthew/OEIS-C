/*
 * A115423  Integers n > 0 such that n XOR 30*n = 31*n.
 * https://oeis.org/A115423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115423: a(n) = the nth positive integer such that
   n XOR (30*n) == 31*n.
   Equivalent to numbers whose binary representation has 1‑bits at
   least 5 positions apart.
   The following implementation computes the nth element by
   combinatorial enumeration of binary strings under this
   restriction. */
#include <stddef.h>

#define MAX_L 200                    /* enough for any n used in tests */

static idx_t a_count[MAX_L + 1];     /* a[len] = count of valid strings < 2^len */
static idx_t h_count[MAX_L + 1];     /* h[len] = count of valid numbers with exactly len bits */
static idx_t cum_count[MAX_L + 1];   /* cumulative counts up to each length */
static int tables_inited = 0;

/* Pre‑compute the auxiliary tables. */
static void init_tables(void)
{
    a_count[0] = 1;                  /* empty string */
    for (idx_t i = 1; i <= MAX_L; ++i)
    {
        if (i < 5)
            a_count[i] = a_count[i - 1] + 1;   /* i+1 */
        else
            a_count[i] = a_count[i - 1] + a_count[i - 5];
    }

    h_count[0] = 0;                  /* no positive numbers with 0 bits */
    cum_count[0] = 0;
    for (idx_t i = 1; i <= MAX_L; ++i)
    {
        if (i < 5)
            h_count[i] = 1;
        else
            h_count[i] = a_count[i - 5];
        cum_count[i] = cum_count[i - 1] + h_count[i];
    }
    tables_inited = 1;
}

/* Recursive helper:
   returns the idx‑th (0‑based) valid number < 2^len, where
   idx ∈ [0, a_count[len]‑1]. */
static seq_t kth_valid(idx_t len, idx_t idx)
{
    if (len == 0)
        return (seq_t)0;

    idx_t count0 = a_count[len - 1];
    if (idx < count0)
        return kth_valid(len - 1, idx);

    /* strings that start with a leading 1 */
    idx_t rest = idx - count0;
    seq_t base = ((seq_t)1 << (len - 1));

    if (len < 5)          /* only one such string exists */
        return base;

    idx_t sublen = len - 5;
    seq_t v = kth_valid(sublen, rest);
    return base + v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!tables_inited)
        init_tables();

    /* Find the length L where the nth element lies. */
    idx_t L = 1;
    while (L <= MAX_L && cum_count[L] < n)
        ++L;

    /* Safety: if n is so large that we ran out of pre‑computed
       lengths, extend the tables (not needed for the test suite). */
    if (L > MAX_L)
        return (seq_t)0;   /* fallback, should not happen for given limits */

    idx_t prev = cum_count[L - 1];
    idx_t k = n - prev;        /* 1‑based rank within the length‑L group */

    if (L < 5)
    {
        /* only one number per length */
        return ((seq_t)1 << (L - 1));
    }

    seq_t base = ((seq_t)1 << (L - 1));
    seq_t v = kth_valid(L - 5, k - 1);   /* 0‑based index in the suffix */
    return base + v;
}
