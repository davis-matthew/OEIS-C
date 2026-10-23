/*
 * A124010  Triangle in which first row is 0, n-th row (n>1) lists the
 *          exponents of distinct prime factors ("ordered prime signature") in
 *          the prime factorization of n.
 * https://oeis.org/A124010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9981, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124010: a(n) is the n‑th element of the flattened
   triangle of prime factor exponents of the integer n.
   Row 1: [0] ; for n > 1: row n contains the exponents
   (in ascending prime order) of the distinct prime factors of n. */

#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------*/
/*  Helpers for the cumulative row lengths                   */
/*------------------------------------------------------------*/
static idx_t *cum = NULL;        /* cumulative lengths: cum[row] = total elements up to this row */
static size_t cum_cap = 0;       /* allocated capacity of cum array (entries) */
static size_t cum_len = 0;       /* last row index computed (i.e. size = cum_len+1) */

/* Count the number of distinct prime factors of m (m >= 2) */
static idx_t count_distinct_prime_factors(idx_t m)
{
    idx_t count = 0;
    while ((m & 1ULL) == 0) {          /* handle factor 2 */
        ++count;
        do { m >>= 1; } while ((m & 1ULL) == 0);
    }
    for (idx_t p = 3; (p * p) <= m; p += 2) {
        if (m % p == 0) {
            ++count;
            do { m /= p; } while (m % p == 0);
        }
    }
    if (m > 1) ++count;
    return count;
}

/* Ensure that cum contains entries up to a row with cumulative
   length >= n.  Returns the last row computed.               */
static void extend_cum_until(idx_t n)
{
    if (!cum) {                       /* first time: allocate */
        cum_cap = 64;
        cum = (idx_t *)malloc((cum_cap + 1) * sizeof(idx_t));
        if (!cum) return;             /* allocation failure, ignore */
        cum[0] = 0;                   /* no elements before row 1 */
        cum_len = 0;
    }

    idx_t cur_row = cum_len;          /* last row whose cum is known */
    idx_t total   = cum[cur_row];

    while (total < n) {
        idx_t next_row = cur_row + 1;
        idx_t len = (next_row == 1) ? 1 : count_distinct_prime_factors(next_row);

        /* grow capacity if necessary */
        if (cum_len >= cum_cap) {
            cum_cap = cum_cap ? cum_cap * 2 : 64;
            idx_t *newptr = (idx_t *)realloc(cum, (cum_cap + 1) * sizeof(idx_t));
            if (!newptr) return;      /* allocation failure, ignore */
            cum = newptr;
        }

        cum_len++;
        cum[cur_row + 1] = total + len;
        total += len;
        cur_row++;
    }
}

/*------------------------------------------------------------*/
/*  Main function for the sequence                          */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined by offset, but harmless */

    /* Prepare cumulative info up to the required row */
    extend_cum_until(n);
    if (!cum) return (seq_t)0;    /* allocation failure */

    /* Find the row number m that contains the n-th element */
    idx_t m = cum_len;                     /* last row computed satisfies cum[m] >= n */
    idx_t offset = n - cum[m - 1] - 1;      /* zero‑based index inside row m */

    if (m == 1) return (seq_t)0;           /* first row is just [0] */

    /* Compute the offset‑th exponent of n=m */
    idx_t x = m;
    idx_t index = 0;
    seq_t exp;

    /* Handle factor 2 */
    if ((x & 1ULL) == 0) {
        exp = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            exp++;
        }
        if (index == offset) return exp;
        index++;
    }

    /* Handle odd factors */
    for (idx_t p = 3; (p * p) <= x; p += 2) {
        if (x % p == 0) {
            exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            if (index == offset) return exp;
            index++;
        }
    }

    /* Remaining prime factor, if any */
    if (x > 1) {               /* exponent is 1 */
        if (index == offset) return (seq_t)1;
        index++;
    }

    /* Should never reach here for valid offset */
    return (seq_t)0;
}
