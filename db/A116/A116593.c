/*
 * A116593  a(n) = b(n+2) + b(n), where b(n) = A006046(n) is the sequence
 *          defined by b(0)=0, b(1)=1, b(n) = 2*b((n-1)/2) + b((n+1)/2) for n
 *          =3,5,7,... and b(n) = 3*b(n/2) for n =2,4,6,....
 * https://oeis.org/A116593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006046(n+2) + A006046(n) * where A006046 is defined by
 *            b(0)=0, b(1)=1, * b(2k) = 3*b(k) * b(2k+1) = 2*b(k)+b(k+1)
 * Range:     u8 n <= 22, u16 n <= 767, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for computed values of b(n)=A006046(n). */
static seq_t *b_cache = NULL;
static size_t b_cache_size = 0;

/* Ensure that b_cache[0..N] are valid. */
static void ensure_b_upto(idx_t N)
{
    if ((size_t)(N + 1) <= b_cache_size)
        return;                           /* already computed */

    size_t new_size = (size_t)(N + 1);
    seq_t *tmp = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));
    if (!tmp)                              /* allocation failed */
        abort();
    b_cache = tmp;

    /* Compute new entries sequentially; all references used here
       are to indices less than the current one, so are already known. */
    for (idx_t i = (idx_t)b_cache_size; i <= N; ++i)
    {
        seq_t val;
        if (i == 0)
            val = 0;
        else if (i == 1)
            val = 1;
        else if ((i & (idx_t)1) == (idx_t)0)   /* even */
            val = 3 * b_cache[i / (idx_t)2];
        else                                   /* odd */
            val = 2 * b_cache[(i - (idx_t)1) / (idx_t)2]
                + b_cache[(i + (idx_t)1) / (idx_t)2];
        b_cache[i] = val;
    }
    b_cache_size = new_size;
}

/* Function required by the build. */
seq_t a(idx_t n)
{
    /* Need b(n) and b(n+2). */
    idx_t need = n + 2;
    ensure_b_upto(need);
    return b_cache[n] + b_cache[n + 2];
}
