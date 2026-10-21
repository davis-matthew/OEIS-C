/*
 * A077438  Numbers k such that Sum_{d|k} mu(d) mu(n/d)^2 = -1.
 * https://oeis.org/A077438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A030059(n)^2, where A030059(n) are the square‑free *
 *            numbers with an odd number of prime factors. The sequence is *
 *            obtained by iterating over all positive integers m, selecting
 *            those * with μ(m) = –1 (square‑free and ω(m) odd), and returning
 *            m² as the * n‑th term. The implementation uses...
 * Range:     u8 n <= 6, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* maximum number for which μ is precomputed.  Enough for the first
 * 10 000 terms (≈ 33 000), we use 200 000 to be safe. */
#define MU_LIMIT 200000

static bool mu_precomputed = false;
static int8_t mu[MU_LIMIT + 1];
static unsigned char is_comp[MU_LIMIT + 1];
static uint32_t primes[MU_LIMIT];
static size_t prime_cnt = 0;

/* precompute Möbius values up to MU_LIMIT using the linear sieve. */
static void init_mu(void)
{
    if (mu_precomputed) return;

    mu[0] = 0;
    mu[1] = 1;
    for (size_t i = 2; i <= MU_LIMIT; ++i) {
        if (!is_comp[i]) {          /* i is prime */
            primes[prime_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > MU_LIMIT) break;
            is_comp[v] = 1;
            if (i % p == 0) {       /* p divides i, square appears */
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    mu_precomputed = true;
}

/* sequence term a(n).  Uses the definition a(n) = m² where m is the
 * n‑th positive integer with μ(m) = –1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined; harmless wrapper */

    init_mu();

    idx_t count = 0;
    uint64_t m = 2;              /* smallest member of A030059 is 2 */

    for (;; ++m) {
        /* For the limits chosen we never exceed MU_LIMIT. */
        if (mu[m] == -1) {       /* m is square‑free and ω(m) odd */
            ++count;
            if (count == n) {
                seq_t res = (seq_t)m * (seq_t)m;
                return res;
            }
        }
    }
}
