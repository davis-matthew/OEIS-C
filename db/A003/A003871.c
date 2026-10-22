/*
 * A003871  Degrees of irreducible representations of symmetric group S_7.
 * https://oeis.org/A003871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest of 7!/prod(hook lengths) over all
 *            partitions of 7 (hook length formula); found by enumerating the
 *            partitions
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t a003871_deg[15];
static int a003871_cnt = 0;

static void a003871_gen(int *parts, int len, int rem, int maxp) {
    if (rem == 0) {
        int conj[7] = {0, 0, 0, 0, 0, 0, 0};
        uint64_t prod = 1;
        int i, j;
        for (i = 0; i < len; i++)
            for (j = 0; j < parts[i]; j++) conj[j]++;
        for (i = 0; i < len; i++)
            for (j = 0; j < parts[i]; j++)
                prod *= (uint64_t)((parts[i] - j - 1) + (conj[j] - i - 1) + 1);
        a003871_deg[a003871_cnt++] = (seq_t)(5040 / prod);
        return;
    }
    {
        int p;
        for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
            parts[len] = p;
            a003871_gen(parts, len + 1, rem - p, p);
        }
    }
}

seq_t a(idx_t n) {
    static bool computed = false;
    if (!computed) {
        int parts[7];
        int i, j;
        a003871_cnt = 0;
        a003871_gen(parts, 0, 7, 7);
        for (i = 0; i < a003871_cnt; i++)
            for (j = i + 1; j < a003871_cnt; j++)
                if (a003871_deg[j] < a003871_deg[i]) {
                    seq_t t = a003871_deg[i];
                    a003871_deg[i] = a003871_deg[j];
                    a003871_deg[j] = t;
                }
        computed = true;
    }
    if (n < 1 || n > (idx_t)a003871_cnt) return 0;
    return a003871_deg[n - 1];
}
