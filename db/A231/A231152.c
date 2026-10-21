/*
 * A231152  Least integer k such that prime(n+1) + ... + prime(n+k) > prime(1)
 *          + ... + prime(n).
 * https://oeis.org/A231152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k such that prime(n+1)+...+prime(n+k) >
 *            prime(1)+...+prime(n)
 * Range:     u8 n <= 9, u16 n <= 1128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Dynamic array helpers: generic reallocation used for primes and cumulative sums. */
static void *grow_vec(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap >= needed) return NULL;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();
    *ptr = tmp;
    *cap = new_cap;
    return tmp;
}

/* Store primes and their running sums. */
static uint32_t *primes = NULL;
static size_t primes_cap = 0, primes_count = 0;
static seq_t *cum = NULL;          /* cum[i] = sum of first i primes, cum[0] = 0 */
static size_t cum_cap = 0, cum_count = 0;

static void grow_primes(size_t needed)   { grow_vec((void**)&primes, &primes_cap, needed, sizeof(uint32_t)); }
static void grow_cum(size_t needed)      { grow_vec((void**)&cum,    &cum_cap,    needed, sizeof(seq_t)); }

static uint32_t next_prime(void)
{
    static uint32_t candidate = 2;
    while (true) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint32_t p = primes[i];
            if ((seq_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint32_t p = candidate;
            ++candidate;
            return p;
        }
        ++candidate;
    }
}

static void add_next_prime(void)
{
    uint32_t p = next_prime();
    grow_primes(primes_count + 1);
    primes[primes_count++] = p;

    if (primes_count == 1) {          /* first prime: initialise cum array */
        grow_cum(2);
        cum[0] = (seq_t)0;
        cum[1] = (seq_t)p;
        cum_count = 2;
    } else {
        grow_cum(cum_count + 1);
        cum[cum_count] = cum[cum_count - 1] + (seq_t)p;
        ++cum_count;
    }
}

static void ensure_primes(size_t needed)
{
    while (primes_count < needed) add_next_prime();
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* defined only for n ≥ 1 */

    /* Need at least n primes (so cum[n] exists). */
    ensure_primes((size_t)n);

    /* Target: find smallest m > n such that cum[m] > 2 * cum[n]. */
    seq_t target = cum[n] * 2;

    /* Extend until cumulative sum exceeds target. */
    while (cum[cum_count - 1] <= target)
        add_next_prime();

    idx_t m = (idx_t)(cum_count - 1);   /* index of first cumulative sum > target */
    return (seq_t)(m - n);              /* a(n) = m - n */
}
