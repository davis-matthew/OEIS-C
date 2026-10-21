/*
 * A068878  Denominators of nonzero terms in the expansion of
 *          sin(x)+exp(x)-1-2*x.
 * https://oeis.org/A068878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3k+1) = (4k+2)!, a(3k+2) = (4k+4)!, a(3k+3) = (4k+5)! / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of factorials.  fact[i] holds (i)! for all 0 <= i < fact_count. */
static seq_t *fact = NULL;   /* array of computed factorials */
static idx_t fact_count = 0; /* number of factorials stored (next unused index) */
static size_t fact_cap = 0;  /* allocated capacity */

/* Ensure that factorials up to index n are computed. */
static void ensure_factorial(idx_t n)
{
    /* Initialise storage if necessary. */
    if (!fact || fact_cap == 0) {
        fact_cap = 64;                     /* initial capacity */
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact) abort();
        fact_count = 0;
    }

    /* Grow the cache to length n+1, computing each new factorial. */
    while (fact_count <= n) {
        if (fact_count == 0) {
            fact[0] = (seq_t)1;             /* 0! = 1 */
        } else {
            fact[fact_count] =
                fact[fact_count - 1] * (seq_t)(fact_count);
        }
        fact_count++;

        /* Resize if the capacity is exceeded. */
        if (fact_count > fact_cap) {
            fact_cap <<= 1;                 /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(fact, fact_cap * sizeof(seq_t));
            if (!tmp) abort();
            fact = tmp;
        }
    }
}

/* Sequence A068878. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n = 0 */

    idx_t k = (n - 1) / 3;        /* 0-based index: n = 3k+1, 3k+2, or 3k+3 */
    idx_t fac_index;              /* index of the factorial needed */
    idx_t mod = n % 3;            /* 0, 1, or 2 */

    if (mod == 1)                 /* n = 3k+1 */
        fac_index = 4 * k + 2;
    else if (mod == 2)            /* n = 3k+2 */
        fac_index = 4 * k + 4;
    else                          /* n = 3k+3 (mod == 0) */
        fac_index = 4 * k + 5;

    ensure_factorial(fac_index);

    seq_t result = fact[fac_index];

    if (mod == 0)                 /* divide by 2 for the (4k+5)!/2 case */
        result /= (seq_t)2;

    return result;
}
