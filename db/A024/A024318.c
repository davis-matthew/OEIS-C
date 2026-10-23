/*
 * A024318  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = A023531, t = (Fibonacci numbers).
 * https://oeis.org/A024318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor((n+1)/2)} A023531(j) * Fibonacci(n-j+1)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023531(j) = 1 if j = k(k+3)/2 for some k≥1, otherwise 0 */

/* Computes the n-th term of OEIS sequence A024318. */
#include <stdlib.h>

static seq_t *fib = NULL;      /* 1‑indexed Fibonacci array (fib[1]=fib[2]=1) */
static idx_t fib_len = 0;      /* highest index that has been initialized */

/* Ensure that fib[1..n] are available.  The array is grown as needed. */
static void ensure_fib(idx_t n)
{
    if (n <= fib_len)
        return;

    idx_t needed = n;
    size_t new_size = (size_t)needed + 1;          /* include index 0 */
    seq_t *tmp = (seq_t *)realloc(fib, new_size * sizeof(seq_t));
    if (!tmp)
        abort();

    fib = tmp;

    if (fib_len == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        if (needed >= 2)
            fib[2] = (seq_t)1;
    } else if (fib_len == 1 && needed >= 2) {
        fib[2] = (seq_t)1;
    }

    idx_t start = fib_len + 1;
    if (start < 3)
        start = 3;
    for (idx_t i = start; i <= needed; ++i)
        fib[i] = fib[i-1] + fib[i-2];

    fib_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* the offset is 1; below it is irrelevant */

    /* maximum Fibonacci index needed is n itself */
    ensure_fib(n);

    idx_t limit = (n + 1) / 2;     /* floor((n+1)/2) */
    seq_t result = (seq_t)0;

    for (idx_t k = 1; ; ++k) {
        /* j = k(k+3)/2  (the positions where A023531 equals 1) */
        idx_t j = (k * (k + 3)) / 2;
        if (j > limit) break;

        idx_t fib_index = n - j + 1;     /* ensures 1 ≤ fib_index ≤ n */
        result += fib[fib_index];
    }

    return result;
}
