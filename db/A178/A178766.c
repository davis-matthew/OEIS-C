/*
 * A178766  Values of gcd(Fibonacci(n), Fibonacci(n+1)+1) sorted with no
 *          repeats.
 * https://oeis.org/A178766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F_{n+1} if n is even * a(n) = F_{n+1} - F_{n-3} if n is
 *            odd (n ≥ 3) * a(1) = 1 * where F_k denotes the k‑th Fibonacci
 *            number with F_0 = 0, F_1 = 1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *fib_cache = NULL;   /* Fibonacci numbers F_0, F_1, ... */
static size_t fib_len = 0;        /* number of Fibonacci numbers stored */
static size_t fib_cap = 0;        /* capacity of fib_cache */

/* Ensure fib_cache contains Fibonacci numbers up to index 'needed'. */
static void ensure_fibs(idx_t needed)
{
    if (fib_len > (size_t)needed)
        return; /* already computed */

    /* Grow capacity */
    if (fib_cap < (size_t)needed + 1) {
        size_t new_cap = fib_cap ? fib_cap << 1 : 16;
        while (new_cap < (size_t)needed + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        fib_cache = tmp;
        fib_cap = new_cap;
    }

    /* Initialise F_0 and F_1 if needed */
    if (fib_len == 0) {
        fib_cache[0] = (seq_t)0;
        fib_len = 1;
    }
    if (fib_len == 1) {
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }

    /* Compute missing Fibonacci numbers */
    for (size_t i = fib_len; i <= (size_t)needed; ++i)
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    fib_len = (size_t)needed + 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1; /* a(1) = 1 */

    /* n≥2: compute needed Fibonacci indices */
    idx_t idx_even = n + 1;          /* used whether even or odd */
    idx_t idx_odd1 = n - 3;          /* used only for odd n≥3 */

    /* Determine maximum index needed */
    idx_t max_needed = idx_even;          /* idx_even >= idx_odd1 for n≥3 */
    ensure_fibs(max_needed);

    if (n % (idx_t)2 == 0) {
        /* even n : a(n) = F_{n+1} */
        return fib_cache[idx_even];
    } else {
        /* odd n (n≥3): a(n) = F_{n+1} - F_{n-3} */
        return fib_cache[idx_even] - fib_cache[idx_odd1];
    }
}
