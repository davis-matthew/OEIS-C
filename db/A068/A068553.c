/*
 * A068553  a(n) = lcm(1,2,...,2*n) / (n*binomial(2*n, n)).
 * https://oeis.org/A068553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(1,2,...,2n) / (n * binomial(2n,n)) (OEIS A068553)
 * Range:     u8 n <= 14, u16 n <= 21, u32 n <= 36, u64 n <= 75, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute primes up to a fixed maximum once */
static uint32_t primes[2500];
static int prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return; /* already initialized */
    const idx_t LIMIT = 20000;            /* 2*10000, sufficient for required n */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return; /* allocation failure, but unlikely */
    for (idx_t i = 0; i <= LIMIT; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (idx_t i = 2; i <= LIMIT; i++)
        if (is_prime[i]) {
            primes[prime_cnt++] = (uint32_t)i;
        }
    free(is_prime);
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    init_primes();  /* ensure primes up to 20000 are ready */

    idx_t two_n = 2 * n;
    seq_t result = (seq_t)1;

    for (int pi = 0; pi < prime_cnt; pi++) {
        uint32_t p = primes[pi];
        if ((idx_t)p > two_n) break;   /* all remaining primes are too big */

        /* exponent of p in lcm(1..2n) = floor(log_p(2n)) */
        int exp_lcm = 0;
        idx_t power = p;
        while (power <= two_n) {
            exp_lcm++;
            power *= p;
        }

        /* exponent of p in binomial(2n,n) */
        int exp_binom = 0;
        power = p;
        while (power <= two_n) {
            exp_binom += (int)( (two_n / power) - 2 * (n / power) );
            power *= p;
        }

        /* exponent of p in n */
        int exp_n = 0;
        idx_t tmp = n;
        while (tmp % p == 0) {
            exp_n++;
            tmp /= p;
        }

        int exp = exp_lcm - exp_binom - exp_n;
        if (exp > 0) {
            for (int k = 0; k < exp; k++) {
                result *= (seq_t)p;
            }
        }
    }
    return result;
}
