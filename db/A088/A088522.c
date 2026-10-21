/*
 * A088522  a(1) = 2; for n > 1, a(n) = (a(n-1) + n) mod prime(n).
 * https://oeis.org/A088522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088522
    Formula: a(1) = 2; for n > 1, a(n) = (a(n-1) + n) mod prime(n)
    where prime(n) is the n-th prime (prime(1)=2). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  Prime generator (trial division with growing list)                */
/* ------------------------------------------------------------------ */
static idx_t *prime_list = NULL;     /* up to how many primes we have */
static size_t prime_count  = 0;      /* number of primes stored       */
static size_t prime_cap    = 0;      /* allocated capacity            */
static idx_t   next_candidate = 3;   /* next odd number to test       */

/* add a prime to the growing list, reallocating if necessary */
static void add_prime(idx_t p) {
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    } else if (prime_count == prime_cap) {
        prime_cap <<= 1;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
    }
    prime_list[prime_count++] = p;
}

/* simple primality test using primes already known */
static bool is_prime_candidate(idx_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1U) == 0) return false;   /* even number */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ensure that at least 'need' primes are stored in prime_list */
static void ensure_primes(idx_t need) {
    if (prime_count >= (size_t)need) return;

    /* 2 is the first prime */
    if (prime_count == 0) add_prime(2);

    while (prime_count < (size_t)need) {
        if (is_prime_candidate(next_candidate))
            add_prime(next_candidate);
        next_candidate += 2;   /* skip even numbers */
    }
}

/* get the n-th prime (n >= 1) */
static idx_t nth_prime(idx_t n) {
    ensure_primes(n);
    return prime_list[n - 1];
}

/* ------------------------------------------------------------------ */
/*  Sequence cache                                               */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;   /* a_cache[1] holds a(1) */
static idx_t a_cache_size = 0;  /* largest index computed */

/* initialise cache lazily */
static void init_cache(void) {
    if (a_cache) return;
    a_cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* indices 0..1 */
    a_cache[0] = (seq_t)0;        /* unused but avoid undefined reference */
    a_cache[1] = (seq_t)2;        /* a(1) = 2 */
    a_cache_size = 1;
}

/* ------------------------------------------------------------------ */
/*  The required sequence function                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of bounds – unused */

    init_cache();

    if (n > a_cache_size) {
        /* extend the cache up to n */
        a_cache = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            seq_t prev = a_cache[i - 1];
            idx_t  p   = nth_prime(i);          /* prime(i)          */
            seq_t  val = (prev + (seq_t)i) % (seq_t)p;
            a_cache[i] = val;
        }
        a_cache_size = n;
    }

    return a_cache[n];
}
