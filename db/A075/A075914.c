/*
 * A075914  Sixth column of triangle A075500.
 * https://oeis.org/A075914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 105*a(n-1) - 4375*a(n-2) + 91875*a(n-3) - 1015000*a(n-4)
 *            + 5512500*a(n-5) - 11250000*a(n-6) with initial values: a(0)=1,
 *            a(1)=105, a(2)=6650, a(3)=330750, a(4)=14266875, a(5)=560896875
 *            This is the linear recurrence derived from the generating
 *            function 1/[(1-5x)(1-10x)(1-15x)(1-20x)(1-25x)(...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cache up to a reasonable maximum (tests use n≈10000) */
    static seq_t cache[10001];
    static int initialized = 0;

    /* first six terms */
    if (!initialized) {
        cache[0] = 1;
        cache[1] = 105;
        cache[2] = 6650;
        cache[3] = 330750;
        cache[4] = 14266875;
        cache[5] = 560896875;
        initialized = 1;
    }

    if (n <= 5) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        /* compute the six components */
        seq_t t1 = 105 * cache[i-1];
        seq_t t2 = 4375 * cache[i-2];
        seq_t t3 = 91875 * cache[i-3];
        seq_t t4 = 1015000 * cache[i-4];
        seq_t t5 = 5512500 * cache[i-5];
        seq_t t6 = 11250000 * cache[i-6];

        /* a[i] = t1 + t3 + t5 - (t2 + t4 + t6) */
        cache[i] = t1;
        cache[i] += t3;
        cache[i] += t5;

        /* subtraction is safe because the result is non‑negative */
        cache[i] -= t2 + t4 + t6;
    }

    return cache[n];
}
