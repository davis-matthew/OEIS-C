/*
 * A011714  A binary m-sequence: expansion of reciprocal of x^8+x^4+x^3+x+1.
 * https://oeis.org/A011714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) is the nth term of the binary m‑sequence defined by the
 *            reciprocal of * x^8 + x^4 + x^3 + x + 1 over GF(2). * * Initial
 *            values: * a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=0, a(7)=1 * * Recurrence for n ≥ 8: * a(n) = a(n-1) XOR
 *            a(n-3) XOR a(n-4) XOR a(n-8) * * The sequence is 51...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, exit */

/* External types are supplied by the build system */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache   = NULL;   /* array of seq_t values   */
    static idx_t max_idx  = 0;      /* highest index already computed */

    /* Initial cache size (arbitrary, grows as needed) */
    const size_t INIT_CAP = 128u;   /* sufficient for many calls */

    /* Initial values for the first 8 terms       */
    const seq_t init[8] = {0, 0, 0, 0, 0, 0, 0, 1};

    /* --- initialize cache if this is the first call --- */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(INIT_CAP * sizeof(seq_t));
        if (cache == NULL)   /* fatal error */
            exit(1);

        /* store the 8 known terms */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];

        /* fill to the initial capacity using the recurrence */
        for (idx_t i = 8; i < INIT_CAP; ++i)
            cache[i] = cache[i-1] ^ cache[i-3] ^ cache[i-4] ^ cache[i-8];

        max_idx = INIT_CAP - 1;
    }

    /* --- extend cache if the requested index is out of range --- */
    if (n > max_idx)
    {
        /* determine needed capacity */
        idx_t need   = n + 1;
        idx_t cap    = max_idx + 1;
        while (cap < need)
            cap <<= 1;                  /* double until big enough */

        /* reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = max_idx + 1; i <= n; ++i)
            cache[i] = cache[i-1] ^ cache[i-3] ^ cache[i-4] ^ cache[i-8];

        max_idx = n;
    }

    return cache[n];
}
