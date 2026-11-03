/*
 * A095281  Upper Wythoff primes, i.e., primes in A001950.
 * https://oeis.org/A095281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime in the upper Wythoff sequence * b_k =
 *            floor(k*φ^2) = floor(k*(φ+1)) = floor(k*φ)+k. * The function
 *            generates successive b_k values, tests each for * primality
 *            (using is_prime_big), and caches the primes found. * a(1) is the
 *            first such prime
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1: a(1) is defined */

    /* cached primes */
    static seq_t *primes = NULL;      /* array of primes */
    static size_t primes_size = 0;    /* number of stored primes */
    static size_t primes_cap  = 0;    /* current allocation capacity */

    /* generation state */
    static idx_t next_k = 1;          /* next k to generate b_k */

    /* constants for φ */
    static const long double sqrt5 = sqrtl(5.0L);
    static const long double phi   = (1.0L + sqrt5) / 2.0L; /* φ */

    /* generate primes until we have at least n of them */
    while (primes_size < n) {
        /* floor(k*φ) computed in long double and cast to idx_t */
        long double a_ld = (long double)next_k * phi + 1e-12L;
        idx_t a_k = (idx_t)a_ld;                /* floor(k*φ) */
        idx_t b_k_uint = a_k + next_k;          /* b_k = floor(k*φ)+k */

        seq_t val = (seq_t)b_k_uint;            /* promote to seq_t */

        if (is_prime_big(val)) {
            grow_seq(&primes, &primes_cap, primes_size + 1);
            primes[primes_size++] = val;
        }

        ++next_k;
    }

    return primes[n - 1];
}
