/*
 * A094589  a(1) = 1; a(n+1) = a(n) + (largest element of {a} <= n).
 * https://oeis.org/A094589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 1 a(n+1) = a(n) + (largest a(k) with a(k) <= n) (OEIS
 *            A094589)
 * Range:     u8 n <= 24, u16 n <= 370, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"   /* provides void grow_seq(seq_t **, size_t *, size_t); */

/* memoised table of generated terms */
static seq_t *seq = NULL;          /* array of a(i)  (seq[0] is a dummy)   */
static size_t cap = 0;             /* current capacity of seq            */
static idx_t max_computed = 0;     /* highest index that has been initialised */
static idx_t kptr = 0;             /* index of largest a(k) <= current threshold */

/* ------------------------------------------------------------------ */
/*  a(n)  --  returns the nth term (n ≥ 1) of OEIS A094589              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – return 0 */

    /* first‑time initialisation ------------------------------------------------ */
    if (seq == NULL) {
        const size_t init_cap = 16;
        grow_seq(&seq, &cap, init_cap);   /* allocate initial storage        */
        seq[0] = (seq_t)0;                /* dummy entry – not used in sum   */
        seq[1] = (seq_t)1;                /* a(1) = 1                        */
        max_computed = 1;
        kptr = 0;                         /* will be advanced to 1 when needed */
    }

    /* if the requested value is already computed -------------------------------- */
    if (n <= max_computed)
        return seq[(size_t)n];

    /* ensure the array is big enough -------------------------------------------- */
    if ((size_t)(n + 1) > cap)
        grow_seq(&seq, &cap, (size_t)(n + 1));

    /* extend the sequence up to n ---------------------------------------------- */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t threshold = (seq_t)(m - 1);     /* we need largest a(k) <= m-1           */
        /* advance kptr while the next stored term is still <= threshold */
        while ((size_t)(kptr + 1) <= (size_t)max_computed &&
               seq[(size_t)(kptr + 1)] <= threshold)
            ++kptr;

        seq[(size_t)m] = seq[(size_t)(m - 1)] + seq[(size_t)kptr];
    }

    max_computed = n;
    return seq[(size_t)n];
}
