/*
 * A157742  A006094(n+3) mod 9.
 * https://oeis.org/A157742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A038194(n+3)*A038194(n+4) mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>


static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t next_candidate = 2;

static void ensure_primes(size_t need)
{
    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[prime_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    /* We need the (n+4)-th prime (1‑based) to compute p_{n+3} and p_{n+4}. */
    size_t need = (size_t)(n + 4);
    ensure_primes(need);

    uint64_t p1 = primes[n + 2]; /* p_{n+3} */
    uint64_t p2 = primes[n + 3]; /* p_{n+4} */

    uint32_t r = (uint32_t)((p1 % 9) * (p2 % 9) % 9);
    return (seq_t)r;
}
