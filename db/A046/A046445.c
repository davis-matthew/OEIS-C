/*
 * A046445  Smallest composite with n prime factors that are distinct in
 *          length.
 * https://oeis.org/A046445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the smallest prime with k digits for k = 1 ..
 *            n+1 ; a(0) = 1 (empty product)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static seq_t *primes = NULL;
static seq_t *prefix = NULL;
static size_t primes_cap = 0;
static size_t prefix_cap = 0;
static size_t primes_cnt = 0;

/* enlarge storage for primes and prefix products */
static void grow(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 4;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_primes = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
    seq_t *new_prefix = (seq_t*)realloc(prefix, new_cap * sizeof(seq_t));
    primes = new_primes;
    prefix = new_prefix;
    primes_cap = new_cap;
}

/* compute the smallest prime with exactly len digits */
static seq_t smallest_prime_with_len(size_t len)
{
    seq_t candidate;
    if (len == 1) {
        candidate = 2;               /* special case */
    } else {
        /* 10^(len-1) */
        candidate = seq_pow((seq_t)10, (idx_t)(len - 1));
    }

    while (!is_prime_big(candidate)) {
        /* candidate++ */
        candidate = candidate + (seq_t)1;
    }
    return candidate;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset 0 */

    size_t need = (size_t)(n + 1);  /* how many primes are required */

    /* generate primes and prefix products up to 'need' */
    while (primes_cnt < need) {
        grow(need);
        size_t idx = primes_cnt + 1;          /* digit length is idx (1‑based) */
        seq_t p = smallest_prime_with_len(idx);
        primes[primes_cnt] = p;
        if (primes_cnt == 0) {
            prefix[primes_cnt] = p;
        } else {
            prefix[primes_cnt] = seq_mul(prefix[primes_cnt - 1], p);
        }
        primes_cnt++;
    }

    return prefix[need - 1];
}
