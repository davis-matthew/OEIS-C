/*
 * A130098  Primes prime(n) such that both of the numbers
 *          (prime(n+2)^2-prime(n)^2)/2 - 1 and (prime(n+2)^2-prime(n)^2)/2 +
 *          1 are not primes.
 * https://oeis.org/A130098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p_k such that both * (prime_{k+2}^2 −
 *            prime_k^2)/2 − 1 * (prime_{k+2}^2 − prime_k^2)/2 + 1 * are not
 *            prime. * The sequence is computed by searching primes in order
 *            and * storing every prime that satisfies the above condition
 * Range:     u8 n <= 1, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static uint64_t *primes = NULL;      /* list of primes found so far          */
static size_t primes_len = 0;        /* number of primes stored              */
static size_t primes_cap = 0;        /* allocated capacity                   */
static uint64_t candidate = 0;       /* current integer to test for primality */

static seq_t *seq = NULL;            /* sequence terms already found         */
static size_t seq_len = 0;           /* number of terms stored               */
static size_t seq_cap = 0;           /* allocated capacity for sequence      */

static void grow_primes(void) {
    if (primes_cap == 0) {
        primes_cap = 16;
    } else {
        primes_cap <<= 1;
    }
    primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
}

static void grow_seq(void) {
    if (seq_cap == 0) {
        seq_cap = 8;
    } else {
        seq_cap <<= 1;
    }
    seq = mem_resize(seq_t, seq, seq_cap);
}

static void add_prime(uint64_t p) {
    if (primes_len == primes_cap) {
        grow_primes();
    }
    primes[primes_len++] = p;
}

static void add_seq(seq_t v) {
    if (seq_len == seq_cap) {
        grow_seq();
    }
    seq[seq_len++] = v;
}

/* Return the nth term of sequence A130098. n is 1‑based; behaviour
 * for n < 1 is undefined. The function is thread‑safe only for
 * single‑threaded use. */
seq_t a(idx_t n) {
    /* Initialise prime list and candidate if first call */
    if (primes == NULL) {
        grow_primes();
        candidate = 2;
    }

    /* Generate primes until we have at least n terms in seq */
    while (seq_len < n) {
        /* Find next prime */
        uint64_t p = 0;
        while (true) {
            if (candidate == 2) {
                p = 2;
                candidate = 3;
                break;
            }
            if ((candidate & 1U) == 0) { /* even numbers are not prime */
                ++candidate;
                continue;
            }
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t pr = primes[i];
                if (pr * pr > candidate) {
                    break;
                }
                if (candidate % pr == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                p = candidate;
                ++candidate;
                break;
            }
            ++candidate;
        }

        /* Store the found prime */
        add_prime(p);

        /* Test the condition if enough primes are available */
        if (primes_len >= 3) {
            size_t k = primes_len - 3;                /* zero‑based index of p_k */
            uint64_t pk = primes[k];
            uint64_t pk2p = primes[k + 2];

            seq_t pk_sq = (seq_t)pk * (seq_t)pk;
            seq_t pk2p_sq = (seq_t)pk2p * (seq_t)pk2p;
            seq_t diff2 = pk2p_sq - pk_sq;           /* (p_{k+2}^2 – p_k^2) */
            seq_t x = diff2 / 2;

            /* both (x-1) and (x+1) must be composite */
            if (!is_prime_big(x - 1) && !is_prime_big(x + 1)) {
                add_seq((seq_t)pk);
            }
        }
    }

    return seq[n - 1];
}
