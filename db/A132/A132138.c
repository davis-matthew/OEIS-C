/*
 * A132138  Characteristic function of the set of numbers defined in A002977.
 * https://oeis.org/A132138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = χ(n) where χ(n) = 1 if n < 3 and n == 1; otherwise χ(n) =
 *            ((n mod 2 == 1) && χ((n-1)/2)) || ((n mod 3 == 1) && χ((n-1)/3))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation (0 = false, 1 = true, 2 = not computed) */
static unsigned char *memo = NULL;
static size_t memo_cap = 0;

/* Ensure the memoisation array can hold values up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < memo_cap) return;   /* already big enough */

    size_t new_cap = memo_cap ? memo_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until big enough */

    unsigned char *new_memo = (unsigned char *)realloc(memo, (new_cap + 1) * sizeof(unsigned char));
    if (!new_memo) { /* allocation failure – abort */
        exit(1);
    }
    memo = new_memo;

    /* Initialise new entries to “not computed” */
    if (memo_cap == 0) {
        memo[0] = 2;
        memo[1] = 1;   /* χ(1) */
        if (new_cap >= 2) memo[2] = 0; /* χ(2) */
        for (size_t i = 3; i <= new_cap; ++i) memo[i] = 2;
    } else {
        for (size_t i = memo_cap + 1; i <= new_cap; ++i) memo[i] = 2;
    }
    memo_cap = new_cap;
}

/* Recursive computation of χ(n) with memoisation */
static bool chi_calc(idx_t n)
{
    if (n < 3) return n == 1;           /* base cases : χ(1)=1 , χ(2)=0 */

    unsigned char val = memo[n];
    if (val != 2) return val;           /* already computed */

    bool res = false;
    if ((n & 1) == 1)                     /* n mod 2 == 1 */
        res |= chi_calc((n - 1) / 2);
    if (!res && (n % 3 == 1))              /* n mod 3 == 1 */
        res |= chi_calc((n - 1) / 3);

    memo[n] = res;                       /* store result for future */
    return res;
}

/* PROTOTYPE REQUIRED BY THE BUILD */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    bool val = chi_calc(n);
    return (seq_t)(val ? 1 : 0);
}
