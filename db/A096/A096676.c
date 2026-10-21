/*
 * A096676  a(n) = (A096788(n)-1)/2.
 * https://oeis.org/A096676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096676: a(n) = (A096788(n)-1)/2.
   A096788(n) are odd composite numbers M for which 2*M+1 is prime.
   Hence a(n) are the integers k such that:
        2*k+1 is a composite odd number
        4*k+3   is a prime.
   The sequence starts: 4, 7, 10, 16, 17, 19, 25, … */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helper: very small primality test (works for 64‑bit) ---------- */
static bool is_prime_u64(idx_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;
    /* check 6k ± 1 up to sqrt(n) using 128‑bit arithmetic to avoid overflow */
    for (idx_t i = 5; (__int128)i * i <= n; i += 6) {
        if (n % i == 0)
            return false;
        if (n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* ---------- cache for computed terms ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_a = 1;          /* next candidate for a(n)               */

/* utility: grow cache array */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap) ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ensure that a(n) for all indices up to 'n' are present in cache */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* check candidate ‘next_a’ */
        idx_t m2 = 2 * next_a + 1;   /* 2*a+1  should be composite   */
        if (is_prime_u64(m2)) {      /* skip if it is prime          */
            ++next_a;
            continue;
        }
        idx_t m4 = 4 * next_a + 3;   /* 4*a+3  must be prime        */
        if (!is_prime_u64(m4)) {
            ++next_a;
            continue;
        }

        /* candidate satisfies both conditions – store it */
        if (cache_len == cache_cap)
            grow_cache();
        cache[cache_len++] = (seq_t)next_a;
        ++next_a;   /* move to next integer for subsequent calls */
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0, but harmless */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
