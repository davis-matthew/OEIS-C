/*
 * A211115  Number of (n+1) X (n+1) -2..2 symmetric matrices with every 2 X 2
 *          subblock having sum zero and two or three distinct values.
 * https://oeis.org/A211115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 16*a(n-3) + 27*a(n-4) + 8*a(n-5) -
 *            35*a(n-6) + 10*a(n-7) + 10*a(n-8) - 4*a(n-9) with initial values
 *            a(1)=12, a(2)=26, a(3)=54, a(4)=106, a(5)=208, a(6)=398,
 *            a(7)=766, a(8)=1458, a(9)=2792
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Lazy static cache for previously computed values */
    static bool initialized = false;
    static seq_t *buf = NULL;   /* 0‑based array: buf[0] == a(1) */
    static idx_t size = 0;      /* number of values stored in buf */
    static idx_t cap   = 0;      /* allocated capacity of buf */

    if (!initialized) {
        const seq_t init[9] = {
            (seq_t)12, (seq_t)26, (seq_t)54,
            (seq_t)106, (seq_t)208, (seq_t)398,
            (seq_t)766, (seq_t)1458, (seq_t)2792
        };
        cap = 32;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) { exit(EXIT_FAILURE); }
        for (idx_t i = 0; i < 9; ++i) buf[i] = init[i];
        size = 9;
        initialized = true;
    }

    if (n <= size) return buf[n-1];

    /* Ensure capacity for up to n terms */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) { exit(EXIT_FAILURE); }
        buf = tmp;
        cap = newcap;
    }

    /* Compute terms from size+1 up to n using the recurrence */
    for (idx_t k = size + 1; k <= n; ++k) {
        idx_t idx = k - 1;          // 0‑based index in buf

        /* Positive contributions */
        seq_t pos = 0;
        pos += (seq_t)5   * buf[idx-1];
        pos += (seq_t)27  * buf[idx-4];
        pos += (seq_t)8   * buf[idx-5];
        pos += (seq_t)10  * buf[idx-7];
        pos += (seq_t)10  * buf[idx-8];

        /* Negative contributions */
        seq_t neg = 0;
        neg += (seq_t)4   * buf[idx-2];
        neg += (seq_t)16  * buf[idx-3];
        neg += (seq_t)35  * buf[idx-6];
        neg += (seq_t)4   * buf[idx-9];

        buf[idx] = pos - neg;      /* pos ≥ neg for all n in this sequence */
    }

    size = n;
    return buf[n-1];
}
