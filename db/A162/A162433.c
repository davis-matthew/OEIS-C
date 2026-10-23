/*
 * A162433  Row 4 of table A162430.
 * https://oeis.org/A162433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} σ( n(n+1)+1 + 2k ) where σ(m) is the sum of
 *            divisors of the odd integer m. This identity follows from the
 *            fact that the number of ordered 4‑tuples of non‑negative
 *            integers (a,b,c,d) with a(a+1)/2 + b(b+1)/2 + c(c+1)/2 +
 *            d(d+1)/2 = m equals σ(2m+1). Thus the desired sequence is...
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 254, u64 n <= 254,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation pre‑computes the primes up to 100 000 once
   and uses trial division to evaluate σ for each odd integer that
   appears in the sum.  The algorithm runs in O(n·π(√max)) time, which
   is well below the 2 s limit for n ≤ 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Prime sieve (once) ---------- */
#define PRIME_CAP 20000          /* enough for all primes ≤ 100 000 */
static int primes[PRIME_CAP];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;          /* already done */

    const int LIMIT = 100000;
    prime_cnt = 0;
    primes[prime_cnt++] = 2;

    for (int i = 3; i <= LIMIT; i += 2) {
        int sqrt_i = (int)sqrt((double)i);
        int is_prime = 1;
        for (size_t j = 0; j < prime_cnt && primes[j] <= sqrt_i; ++j) {
            if (i % primes[j] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_cnt >= PRIME_CAP) break; /* safety */
            primes[prime_cnt++] = i;
        }
    }
}

/* ---------- Sum of divisors for an odd integer ---------- */
static seq_t sigma_odd(seq_t n)
{
    init_primes();

    seq_t result = 1;
    seq_t temp = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        /* Stop once p*p > temp */
        if (p * p > temp) break;

        if (temp % p == 0) {
            seq_t term = 1;   /* geometric series for this prime factor */
            seq_t power = 1;
            do {
                temp /= p;
                power *= p;
                term += power;
            } while (temp % p == 0);
            result *= term;
        }
    }

    if (temp > 1) {               /* remaining prime factor > 1 */
        result *= (1 + temp);
    }

    return result;
}

/* ---------- Sequence a(n) ---------- */
seq_t a(idx_t n)
{
    seq_t total = 0;

    /* lower odd integer L = n(n+1)+1 */
    seq_t L = (seq_t)n * (n + 1) + 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t x = L + (seq_t)(2 * k);
        total += sigma_odd(x);
    }

    return total;
}
