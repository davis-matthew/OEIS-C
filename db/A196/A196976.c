/*
 * A196976  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,2,1,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=5, a(3)=14, a(4)=37, a(5)=104, a(6)=290; for n≥7:
 *            a(n)=2*a(n‑1)+a(n‑2)+4*a(n‑3) – 2*a(n‑4) – a(n‑6)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed values.
   It works for any unsigned integer type or arbitrary‑precision type
   assigned to seq_t and any unsigned 64‑bit type assigned to idx_t. */
#include <stdlib.h>

static seq_t *cache = NULL;        /* array storing a(1)…a(n)  */
static size_t cached_up_to = 0;    /* number of terms currently cached */

/* Ensure that cache contains a(1)…a(n).  The function may be called in any
   order of n and is safe for repeated calls. */
static void extend_to(idx_t n)
{
    if ((size_t)n <= cached_up_to)                /* already have everything we need */
        return;

    size_t new_size = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        abort();                                  /* allocation failed */
    cache = tmp;

    /* Initial terms a(1) … a(6) */
    static const seq_t base[6] = { (seq_t)1, (seq_t)5, (seq_t)14,
                                   (seq_t)37, (seq_t)104, (seq_t)290 };

    for (size_t i = cached_up_to; i < new_size; ++i) {
        seq_t val;
        if (i < 6) {                              /* use the known initial values */
            val = base[i];
        } else {
            /* recurrence: a(n) = 2*a(n‑1)+a(n‑2)+4*a(n‑3) – 2*a(n‑4) – a(n‑6) */
            seq_t a1 = cache[i-1];                /* a(n‑1)  */
            seq_t a2 = cache[i-2];                /* a(n‑2)  */
            seq_t a3 = cache[i-3];                /* a(n‑3)  */
            seq_t a4 = cache[i-4];                /* a(n‑4)  */
            seq_t a6 = cache[i-6];                /* a(n‑6)  */
            val = a1 * 2U + a2 + a3 * 4U - a4 * 2U - a6;
        }
        cache[i] = val;                            /* store a(i+1) */
    }

    cached_up_to = new_size;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 but harmless */
    extend_to(n);                  /* ensure a(n) is cached */
    return cache[n-1];
}
