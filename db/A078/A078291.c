/*
 * A078291  Least nontrivial multiple of the n-th prime beginning with 7.
 * https://oeis.org/A078291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least non‑trivial multiple of the n‑th prime whose
 *            decimal representation begins with 7
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 70, a(2) = 72, a(3) = 70, ... as shown in A078291 */
#include <stdlib.h>
#include <stdbool.h>

/* Cached list of primes, 1‑based index: primes[0] = 2, primes[1] = 3, ... */
static idx_t *primes = NULL;        /* array of primes */
static size_t primes_len = 0;       /* number of primes stored */
static size_t primes_cap = 0;       /* allocated capacity */

/* Add a prime to the cache, expanding the array if needed */
static void add_prime(idx_t p) {
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) return;  /* silently fail – caller will not succeed */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Ensure that primes[0 .. up_to-1] are available (up_to is 1‑based count). */
static void ensure_primes(size_t up_to) {
    if (primes_len == 0) {
        add_prime((idx_t)2);          /* first prime */
    }
    while (primes_len < up_to) {
        /* search for the next prime after the current maximum */
        idx_t candidate = primes[primes_len - 1] + 1;
        if (candidate % 2 == 0) ++candidate;  /* skip even numbers */

        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                idx_t p = primes[i];
                if ((idx_t)p * (idx_t)p > candidate) break; /* enough tested */
                if (candidate % p == 0) { isprime = false; break; }
            }
            if (isprime) {
                add_prime(candidate);
                break;
            }
            candidate += 2;  /* next odd candidate */
        }
    }
}

/* Return the most significant decimal digit of x. Assumes seq_t has division by 10. */
static int leading_digit(seq_t x) {
    while (x >= (seq_t)10) {
        x = x / (seq_t)10;
    }
    return (int)x;
}

/* Main function required by the rules. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety – offset is 1 */
    ensure_primes((size_t)n);
    idx_t p = primes[(size_t)n - 1];     /* p_n, 1‑based */

    for (idx_t k = 2; ; ++k) {           /* k must be >1 (non‑trivial multiple) */
        /* Compute product in a wide intermediate when possible */
        __int128 prod128 = (__int128)p * (__int128)k;
        seq_t prod = (seq_t)prod128;      /* may truncate if seq_t < 128 bits,
                                            but the problem guarantees fits for all
                                            requested n and seq_t size */

        if (leading_digit(prod) == 7) {
            return prod;
        }
    }
}
