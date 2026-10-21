/*
 * A097497  Floor( prime(n)*(prime(n)+prime(n+1))/prime(n+1)).
 * https://oeis.org/A097497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = ⌊ prime(n) · (prime(n)+prime(n+1)) / prime(n+1) ⌋
 *            Simplified: a(n) = prime(n) + ⌊ prime(n)² / prime(n+1) ⌋ The
 *            function below generates successive primes on demand by simple
 *            trial division using the primes already found. For n up to ~10
 *            000 this is easily fast enough (well below the 2 s li...
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* stored primes (0‑based index corresponds to prime(1)=2) */
static uint32_t *prime_list = NULL;   /* dynamically expanded */
static size_t   prime_cnt  = 0;        /* number of primes stored */

/* Add the next prime to prime_list.  Uses trial division by
   previously found primes up to sqrt(candidate). */
static bool add_next_prime(void)
{
    uint32_t candidate;
    if (prime_cnt == 0)
        candidate = 2;
    else
        candidate = prime_list[prime_cnt - 1] + 1;

    while (true) {
        /* check divisibility */
        bool is_prime = true;
        uint32_t limit = (uint32_t)(candidate * 1.0 / 1); /* dummy to avoid division by zero */
        /* sqrt(candidate) ≤ 2^16 for candidate ≤ UINT32_MAX, but will never get that high */
        uint32_t sqrt_cand = (uint32_t)(candidate);
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* ensure capacity */
            if (prime_cnt == 0) {
                prime_list = (uint32_t*)malloc(64 * sizeof(uint32_t));
                if (!prime_list) return false;
                prime_cnt = 0;
            } else if (prime_cnt % 64 == 0) {
                uint32_t *tmp = (uint32_t*)realloc(prime_list, (prime_cnt + 64) * sizeof(uint32_t));
                if (!tmp) return false;
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = candidate;
            return true;
        }
        ++candidate;
    }
    return false; /* unreachable */
}

static void ensure_primes(size_t need_idx)
{
    while (prime_cnt <= need_idx) {
        if (!add_next_prime())
            break; /* should not happen */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1, return 0 */

    /* we need prime(n) and prime(n+1) => indices n-1 and n in 0‑based array */
    ensure_primes((size_t)n);

    uint32_t pn   = prime_list[(size_t)n - 1];
    uint32_t pn1  = prime_list[(size_t)n];

    /* compute result using seq_t for arbitrary precision */
    seq_t p  = (seq_t)pn;
    seq_t p1 = (seq_t)pn1;
    seq_t result = p + (p * p) / p1;   /* floor division truncates */

    return result;
}
