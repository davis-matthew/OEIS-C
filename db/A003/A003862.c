/*
 * A003862  Degrees of irreducible representations of alternating group A_7.
 * https://oeis.org/A003862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible representations of A_7, sorted
 *            ascending. They are found from the partitions lambda of 7. Let
 *            f(lambda) = 7!/prod(hook lengths). If lambda differs from its
 *            conjugate, the pair {lambda, lambda'} gives one A_7 irrep of
 *            degree f(lambda). If lambda is self-conjugate, it giv...
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define A3862_N 7

static int a3862_done = 0;
static long long a3862_deg[32];
static int a3862_cnt = 0;
static int a3862_part[A3862_N + 1];

static void a3862_process(int len)
{
    int conj[A3862_N + 1];
    int i, j, c;
    long long prod = 1, fact = 1, f;
    for (i = 0; i <= A3862_N; i++) conj[i] = 0;
    for (j = 0; j < a3862_part[0]; j++) {
        c = 0;
        for (i = 0; i < len; i++)
            if (a3862_part[i] > j) c++;
        conj[j] = c;
    }
    for (i = 0; i < len; i++)
        for (j = 0; j < a3862_part[i]; j++)
            prod *= (long long)(a3862_part[i] - j + conj[j] - i - 1);
    for (i = 2; i <= A3862_N; i++) fact *= i;
    f = fact / prod;
    /* lexicographic comparison of lambda with its conjugate */
    c = 0;
    for (i = 0; i < A3862_N; i++) {
        int x = (i < len) ? a3862_part[i] : 0;
        if (x != conj[i]) { c = (x > conj[i]) ? 1 : -1; break; }
    }
    if (c == 0) {
        a3862_deg[a3862_cnt++] = f / 2;
        a3862_deg[a3862_cnt++] = f / 2;
    } else if (c > 0) {
        a3862_deg[a3862_cnt++] = f;
    }
}

static void a3862_rec(int rem, int maxp, int len)
{
    int p;
    if (rem == 0) { a3862_process(len); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        a3862_part[len] = p;
        a3862_rec(rem - p, p, len + 1);
    }
}

static void a3862_init(void)
{
    int i, j;
    a3862_cnt = 0;
    a3862_rec(A3862_N, A3862_N, 0);
    for (i = 1; i < a3862_cnt; i++) {
        long long v = a3862_deg[i];
        j = i - 1;
        while (j >= 0 && a3862_deg[j] > v) {
            a3862_deg[j + 1] = a3862_deg[j];
            j--;
        }
        a3862_deg[j + 1] = v;
    }
    a3862_done = 1;
}

seq_t a(idx_t n)
{
    if (!a3862_done) a3862_init();
    if (n < 1 || n > (idx_t)a3862_cnt) return (seq_t)0;
    return (seq_t)a3862_deg[n - 1];
}
