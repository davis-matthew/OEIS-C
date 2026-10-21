/*
 * A204480  Number of (n+2)X7 0..3 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A204480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1)+a(n-2)-2*a(n-3)-19*a(n-4)-4*a(n-5)+
 *             21*a(n-6)+24*a(n-7)+3*a(n-8)-18*a(n-9)-9*a(n-10)  for n>13
 * Initial terms:
 *   a(1)= 7191
 *   a(2)= 8114
 *   a(3)= 2632
 *   a(4)= 3270
 *   a(5)= 6494
 *   a(6)=11338
 *   a(7)=35990
 *   a(8)=132258
 *   a(9)=473380
 *   a(10)=1786812
 *   a(11)=6773680
 *   a(12)=25609458
 *   a(13)=97080840
 *
 * This implementation uses a static cache sized to support
 * n up to 10000 (larger values will return 0).  It works for
 * both compiled as C (where seq_t is a built‑in unsigned type)
 * and compiled as C++ (where seq_t may be a large integer class).
 */

#include <stddef.h>  /* for size_t */

#define MAX_N 10000

seq_t a(idx_t n)
{
    /* Handle unused indices below the offset */
    if (n == (idx_t)0) return 0;

    /* Static cache, initialisation guard, and highest computed index */
    static seq_t cache[MAX_N + 1];
    static size_t init_done = 0;      /* 0 = not initialised, 1 = ready */
    static size_t computed_up_to = 13;/* last index with a defined value */

    /* Initialise the cache once */
    if (init_done == 0) {
        /* Zero all entries for safety */
        for (size_t i = 0; i <= MAX_N; ++i) cache[i] = (seq_t)0;

        /* Base cases a(1) .. a(13) */
        cache[1]  = (seq_t)7191;
        cache[2]  = (seq_t)8114;
        cache[3]  = (seq_t)2632;
        cache[4]  = (seq_t)3270;
        cache[5]  = (seq_t)6494;
        cache[6]  = (seq_t)11338;
        cache[7]  = (seq_t)35990;
        cache[8]  = (seq_t)132258;
        cache[9]  = (seq_t)473380;
        cache[10] = (seq_t)1786812;
        cache[11] = (seq_t)6773680;
        cache[12] = (seq_t)25609458;
        cache[13] = (seq_t)97080840;
        init_done = 1;
    }

    /* Reject indices that are too large for the static cache */
    if (n > (idx_t)MAX_N) return (seq_t)0;

    /* Return cached value if it already exists */
    size_t ui = (size_t)n;
    if (ui <= computed_up_to) return cache[ui];

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = computed_up_to + 1; i <= ui; ++i) {
        seq_t t = 0;
        /* Positive contributions */
        t += (seq_t)4 * cache[i-1];
        t += (seq_t)1 * cache[i-2];
        t += (seq_t)21 * cache[i-6];
        t += (seq_t)24 * cache[i-7];
        t += (seq_t)3  * cache[i-8];
        /* Negative contributions */
        t -= (seq_t)2  * cache[i-3];
        t -= (seq_t)19 * cache[i-4];
        t -= (seq_t)4  * cache[i-5];
        t -= (seq_t)18 * cache[i-9];
        t -= (seq_t)9  * cache[i-10];
        cache[i] = t;
    }
    computed_up_to = ui;
    return cache[ui];
}
