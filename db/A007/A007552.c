/*
 * A007552  Exponentiation of Fibonacci numbers.
 * https://oeis.org/A007552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1)*Fibonacci(k+1)*a(n-k),
 *            a(0) = 1 (from E.g.f. exp(B(x))-1 where B(x) = Sum_{k>=1}
 *            Fibonacci(k+1)*x^k/k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A_tab = 0;   /* A_tab[0..done] known */
static seq_t *F_tab = 0;   /* Fibonacci numbers */
static seq_t *row = 0;     /* row[j] = binomial(done-1, j) */
static idx_t done = 0;
static idx_t cap = 0;
static idx_t fib_len = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    A_tab = grow_zero(A_tab, cap, nc);
    F_tab = grow_zero(F_tab, cap, nc);
    row = grow_zero(row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    ensure_cap(n + 3);
    if (fib_len == 0) {
        F_tab[0] = 0;
        F_tab[1] = 1;
        fib_len = 2;
        A_tab[0] = 1;
        done = 0;
    }
    while (fib_len <= n + 1) {
        F_tab[fib_len] = F_tab[fib_len - 1] + F_tab[fib_len - 2];
        fib_len++;
    }
    while (done < n) {
        idx_t N = done;
        /* update row to binomial(N, .) */
        row[N] = 1;
        for (idx_t j = N; j-- > 1;) row[j] += row[j - 1];
        seq_t s = 0;
        for (idx_t k = 1; k <= N + 1; k++)
            s += row[k - 1] * F_tab[k + 1] * A_tab[N + 1 - k];
        A_tab[N + 1] = s;
        done = N + 1;
    }
    return A_tab[n];
}
