/*
 * A003872  Degrees of irreducible representations of symmetric group S_8.
 * https://oeis.org/A003872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest of 8!/prod(hook lengths of lambda) over the
 *            22 partitions lambda of 8 (hook length formula), sorted
 *            ascending
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static long long a003872_deg[32];
static int a003872_cnt = 0;
static bool a003872_ready = false;

static void a003872_gen(int rem, int maxp, int *p, int k)
{
    if (rem == 0) {
        int c[9];
        int i, j;
        long long num = 40320, den = 1;
        for (j = 0; j < 9; j++) {
            c[j] = 0;
            for (i = 0; i < k; i++)
                if (p[i] > j) c[j]++;
        }
        for (i = 0; i < k; i++)
            for (j = 0; j < p[i]; j++)
                den *= (long long)((p[i] - j - 1) + (c[j] - i - 1) + 1);
        a003872_deg[a003872_cnt++] = num / den;
        return;
    }
    {
        int part;
        for (part = (rem < maxp ? rem : maxp); part >= 1; part--) {
            p[k] = part;
            a003872_gen(rem - part, part, p, k + 1);
        }
    }
}

seq_t a(idx_t n)
{
    if (!a003872_ready) {
        int p[9];
        int i, j;
        a003872_cnt = 0;
        a003872_gen(8, 8, p, 0);
        for (i = 1; i < a003872_cnt; i++) {
            long long v = a003872_deg[i];
            j = i - 1;
            while (j >= 0 && a003872_deg[j] > v) {
                a003872_deg[j + 1] = a003872_deg[j];
                j--;
            }
            a003872_deg[j + 1] = v;
        }
        a003872_ready = true;
    }
    if (n < 1 || n > (idx_t)a003872_cnt) return (seq_t)0;
    return (seq_t)a003872_deg[n - 1];
}
