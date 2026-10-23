/*
 * A024481  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (1, p(1), p(2), ...), t = (F(2), F(3), ...).
 * https://oeis.org/A024481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{floor((n+1)/2)} s(i) * t(n+1-i), where s(1)=1,
 *            s(i)=p(i-1) (primes), and t(i)=F(i+1) (Fibonacci, F1=1)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Allocation helpers – work for both C11 and C++17.                      */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
/* In C++ use new/delete so that arithmetic type or arbitrary‑precision
   class constructors/destructors are correctly invoked. */
static inline seq_t* seq_alloc(idx_t sz) { return new seq_t[sz]; }
static inline void seq_free(seq_t *p)       { delete[] p; }

static inline uint64_t* uint64_alloc(idx_t sz) { return new uint64_t[sz]; }
static inline void uint64_free(uint64_t *p)     { delete[] p; }
#else
/* In C use malloc/free. For unsigned integer types this is safe. */
static inline seq_t* seq_alloc(idx_t sz) { return (seq_t*)malloc(sz * sizeof(seq_t)); }
static inline void seq_free(seq_t *p)       { free(p); }

static inline uint64_t* uint64_alloc(idx_t sz) { return (uint64_t*)malloc(sz * sizeof(uint64_t)); }
static inline void uint64_free(uint64_t *p)     { free(p); }
#endif

/* --------------------------------------------------------------------- */
/*  Simple prime generator – generates the first k primes (k>=1).        */
/* --------------------------------------------------------------------- */
static void generate_primes(uint64_t *array, idx_t k)
{
    idx_t count = 0;
    uint64_t cand = 2;
    while (count < k) {
        bool is_prime = true;
        for (idx_t j = 0; j < count; ++j) {
            uint64_t p = array[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) array[count++] = cand;
        ++cand;
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence function.                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – behaviour for n<1 is irrelevant */

    idx_t k = (n + 1) / 2;         /* floor((n+1)/2) */

    /* Generate first k primes. */
    uint64_t *primes = uint64_alloc(k);
    generate_primes(primes, k);

    /* Compute Fibonacci numbers t(i) = F(i+1) for i = 1 .. n+1. */
    seq_t *fib = seq_alloc(n + 2);   /* indices 0..n+1, we use 1..n+1 */
    fib[1] = (seq_t)1;              /* t(1) = F(2) = 1 */
    if (n + 1 >= 2) fib[2] = (seq_t)2; /* t(2) = F(3) = 2 */
    for (idx_t i = 3; i <= n + 1; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    /* Compute weighted sum. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s_val = (i == 1) ? (seq_t)1 : (seq_t)primes[i - 2];
        seq_t t_val = fib[n + 1 - i];
        sum += s_val * t_val;
    }

    /* Clean up. */
    seq_free(fib);
    uint64_free(primes);

    return sum;
}
