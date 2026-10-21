/*
 * A223694  Petersen graph (8,2) coloring a rectangular array: number of 3Xn
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal or antidiagonal ne...
 * https://oeis.org/A223694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4096, a(2)=3888, a(3)=37008, a(4)=363600, a(5)=3788640,
 *            a(6)=40075632, a(7)=427910688, a(8)=4599435024,
 *            a(9)=49661922528, a(10)=537886587312, a(11)=5838098127264,
 *            a(12)=63455538372048. For n > 12: a(n) = 13*a(n-1) + 15*a(n-2)
 *            -391*a(n-3) -399*a(n-4) +1739*a(n-5) +205*a(n-6) -2013*a(n-7)
 *            +580...
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        4096ull, 3888ull, 37008ull, 363600ull, 3788640ull,
        40075632ull, 427910688ull, 4599435024ull, 49661922528ull,
        537886587312ull, 5838098127264ull, 63455538372048ull
    };

    if (n <= 12) return init[n-1];

    /* Hold the last 10 terms a[n-10] .. a[n-1] */
    seq_t prev[10];
    /* Initialize with a[3] .. a[12] */
    for (idx_t i = 0; i < 10; ++i)
        prev[i] = init[2 + i];   /* offset 2 -> a3 */

    seq_t result = 0;
    for (idx_t cur = 13; cur <= n; ++cur) {
        /* compute a[cur] from prev[0..9] */
        result = 0;
        result += 13 * prev[9];   /* a[n-1] */
        result += 15 * prev[8];   /* a[n-2] */
        result += 1739 * prev[5]; /* a[n-5] */
        result += 205 * prev[4];  /* a[n-6] */
        result += 580 * prev[2];  /* a[n-8] */
        result += 396 * prev[1];  /* a[n-9] */

        result -= 391 * prev[7];  /* a[n-3] */
        result -= 399 * prev[6];  /* a[n-4] */
        result -= 2013 * prev[3]; /* a[n-7] */
        result -= 144 * prev[0];  /* a[n-10] */

        /* shift prev to prepare for next iteration */
        for (idx_t i = 0; i < 9; ++i)
            prev[i] = prev[i+1];
        prev[9] = result;
    }
    return result;
}
