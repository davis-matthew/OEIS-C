/*
 * A179396  a(n) = prime(n)^2 modulo prime(n+4).
 * https://oeis.org/A179396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 mod prime(n+4)
 * Range:     u8 n <= 58, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a small sieve of Eratosthenes to obtain primes on demand.
   The function works for any seq_t/idx_t pair supported by the build. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

static idx_t    *prime_list = NULL;   /* 1‑based indexed array of primes  */
static size_t    prime_count = 0;     /* number of primes stored           */
static idx_t     prime_limit = 0;     /* current sieve bound               */

/* Ensure that at least 'need' primes are available. */
static void ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    idx_t new_limit = _prime_limit_for_idx(need);
    if (new_limit <= prime_limit)
        new_limit = prime_limit * 2 + 100;   /* enlarge if necessary */

    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes(new_limit, &new_primes, &new_cnt);

    /* Replace old sieve with the new one. */
    if (prime_list)
        free(prime_list);

    prime_list  = new_primes;
    prime_count = new_cnt;
    prime_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for n < 1, return 0 */
        return 0;

    /* We need primes up to index n+4. */
    ensure_primes(n + 4);

    /* Retrieve required primes (1‑based indexing). */
    idx_t p_n   = prime_list[n      - 1];   /* prime(n)   */
    idx_t p_np4 = prime_list[n + 4 - 1];   /* prime(n+4) */

    /* Compute p_n^2 mod p_np4 using 64‑bit arithmetic for safety. */
    uint64_t mod  = (uint64_t)p_np4;
    uint64_t base = (uint64_t)p_n;
    uint64_t result = (base * base) % mod;

    return (seq_t)result;
}
