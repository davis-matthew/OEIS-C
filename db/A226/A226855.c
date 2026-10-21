/*
 * A226855  a(n) = n*B(n-1) + n*(n-1)*B(n-2), where the B(i) are Bell numbers
 *          (A000110).
 * https://oeis.org/A226855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*B(n-1) + n*(n-1)*B(n-2) with B(0)=1. a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation */
#include <stdlib.h>
#include <stddef.h>

/* Static array of Bell numbers (B[0] = 1) */
static seq_t *Bell = NULL;          /* array of Bell numbers */
static size_t Bell_size = 0;         /* number of initialized entries */
static size_t Bell_cap  = 0;         /* allocated capacity */

/* Grow the Bell array to at least new_cap elements */
static void grow_bell(size_t new_cap)
{
    if (new_cap <= Bell_cap)
        return;
    size_t cap = Bell_cap ? Bell_cap * 2 : 4;
    if (cap < new_cap)
        cap = new_cap;
    Bell = (seq_t *)realloc(Bell, cap * sizeof(seq_t));
    /* In the unlikely event of allocation failure, Bell will be NULL
       and the program will crash when accessed – acceptable for this
       context. */
    Bell_cap = cap;
}

/* Compute Bell numbers up to index N (inclusive) */
static void compute_bell_up_to(idx_t N)
{
    if (Bell_size == 0) {
        /* Ensure at least one entry for B(0) */
        grow_bell(1);
        Bell[0] = 1;      /* B(0) = 1 */
        Bell_size = 1;
    }

    if (N < (idx_t)Bell_size)
        return;

    grow_bell((size_t)(N + 1));

    for (idx_t i = Bell_size; i <= N; ++i) {
        seq_t sum = 0;
        seq_t bincoeff = 1;       /* C(i-1,0) */
        idx_t base = i - 1;
        for (idx_t k = 0; k <= base; ++k) {
            sum += bincoeff * Bell[k];
            if (k < base)
                bincoeff = bincoeff * (base - k) / (k + 1);
        }
        Bell[i] = sum;
    }
    Bell_size = (size_t)(N + 1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) = 0 */
    if (n == 1)
        return 1;          /* a(1) = 1 */

    /* Ensure required Bell numbers are available */
    compute_bell_up_to(n - 1);

    seq_t bn1 = Bell[n - 1];         /* B(n-1) */
    seq_t bn2 = Bell[n - 2];         /* B(n-2) */

    seq_t term1 = (seq_t)n * bn1;
    seq_t term2 = (seq_t)n * (seq_t)(n - 1) * bn2;
    return term1 + term2;
}
