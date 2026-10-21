/*
 * A093322  Tetranacci numbers starting with first four cubes.
 * https://oeis.org/A093322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + a(n‑3) + a(n‑4) with a(1) = 1, a(2) =
 *            8, a(3) = 27, a(4) = 64
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 65, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *buf = NULL;      /* memoised values, 1‑based */
static idx_t  cap = 0;         /* allocated capacity (0 if unallocated) */
static idx_t  last = 0;        /* largest n whose value is computed */

/* Ensure array has space for index n (1‑based). */
static void ensure_cap(idx_t n) {
    if (n >= cap) {
        idx_t newcap = cap ? cap * 2 : 16;
        while (newcap <= n) newcap <<= 1;                /* newcap = max(16, next power of 2 ≥ n+1) */

        /* realloc may return void*; cast to seq_t* to satisfy C++ compilers. */
        seq_t *tmp = (seq_t*)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            exit(1);
        }
        buf = tmp;
        /* zero‑initialize new elements to a safe default for seq_t */
        memset((void*)(buf + cap), 0, (newcap - cap) * sizeof(seq_t));

        if (cap <= 4) {
            buf[1] = (seq_t)1;
            buf[2] = (seq_t)8;
            buf[3] = (seq_t)27;
            buf[4] = (seq_t)64;
        }
        cap = newcap;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* offset 1; guard against underflow */

    if (n <= 4) {   /* initial values */
        switch (n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)8;
            case 3: return (seq_t)27;
            case 4: return (seq_t)64;
        }
    }

    ensure_cap(n);

    if (n > last) {
        idx_t start = (last < 5) ? 5 : last + 1;
        for (idx_t i = start; i <= n; ++i) {
            buf[i] = buf[i-1] + buf[i-2] + buf[i-3] + buf[i-4];
        }
        last = n;
    }
    return buf[n];
}
