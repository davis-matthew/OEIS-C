/*
 * A103374  a(1) = a(2) = a(3) = a(4) = a(5) = a(6) = a(7) = 1 and for n>7:
 *          a(n) = a(n-6) + a(n-7).
 * https://oeis.org/A103374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 107, u32 n <= 211, u64 n <= 419,
 *            u128 n <= 834, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103374: a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=a(7)=1,
   a(n)=a(n-6)+a(n-7) for n>7.  */
#include <stdlib.h>   /* for size_t, NULL */
#include <stdint.h>   /* for uint64_t */

/*  Stored values.  Must be valid C11 and C++17.  */
static seq_t *memo = NULL;   /* array of coefficients */
static idx_t memo_len = 0;   /* current allocated size */

/*  Initialise the table with the seven initial values.  */
static void init(void)
{
    if (memo != NULL)
        return;

    memo_len = 8; /* indices 0..7 inclusive, 0 unused */

#ifdef __cplusplus
    /* C++: allocate and value‑initialise objects. */
    memo = new seq_t[memo_len];
    for (idx_t i = 0; i < memo_len; ++i)
        memo[i] = 0;
#else
    /* C: allocate zeroed memory. */
    memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
#endif
    /* 0th index is unused; set the seven known values. */
    for (idx_t i = 1; i <= 7; ++i)
        memo[i] = 1;
}

/*  Extend the table up to index n (inclusive).  */
static void extend(idx_t n)
{
    if (n + 1 <= memo_len)
        return;                     /* already computed */

    idx_t new_len = n + 1;          /* need indices 0..n inclusive */

    seq_t *tmp = NULL;
#ifdef __cplusplus
    /* Allocate new array and copy existing values. */
    tmp = new seq_t[new_len];
    for (idx_t i = 0; i < memo_len; ++i)
        tmp[i] = memo[i];
    /* Delete old array. */
    delete[] memo;
#else
    /* C: reallocate and keep old contents. */
    tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
    if (tmp == NULL)                /* allocation failed */
        return;
    memo = tmp;
#endif
    /* Initialise new cells to zero (C++ ctor already did that). */
    for (idx_t i = memo_len; i < new_len; ++i)
        tmp[i] = 0;

    /* Fill in the recurrence.  The loop starts at i=memo_len,
       which is >=8 because memo_len was at least 8. */
    for (idx_t i = memo_len; i <= n; ++i)
        tmp[i] = tmp[i - 6] + tmp[i - 7];

    memo = tmp;
    memo_len = new_len;
}

/*  External linkage required by the test harness.  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n <= 0, return 0. */

    init();
    if ((n + 1) > memo_len)
        extend(n);

    return memo[n];
}
