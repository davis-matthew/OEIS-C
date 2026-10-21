/*
 * A061398  Number of squarefree integers between prime(n) and prime(n+1).
 * https://oeis.org/A061398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squarefree integers m with prime(n) < m <
 *            prime(n+1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a sieve of primes up to 200 000 and the Möbius function up to 1 000.
 *  Square‑free count ≤ X is obtained by
 *      S(X) = ∑_{k=1}^{⌊√X⌋} μ(k)·⌊X/k²⌋
 *  Then a(n) = S(prime(n+1)) – S(prime(n)) – 1.  */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Static tables                                                    */
/* ------------------------------------------------------------------ */
static int initialized = 0;

#define PRIME_LIMIT   200000   /* enough for n ≤ 10 000          */
#define MAX_PRIMES    20000    /* number of primes up to limit   */
#define MU_MAX        1000     /* max k for Möbius function     */

static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static int8_t mu[MU_MAX + 1];
static size_t mu_count = 0;

/* ------------------------------------------------------------------ */
/*  Initialisation of primes and Möbius table                          */
/* ------------------------------------------------------------------ */
static void init_tables(void) {
    if (initialized)
        return;

    /* sieve of Eratosthenes for primes */
    static uint8_t is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = 0;  is_prime[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[prime_count++] = (uint32_t)i;

    /* linear sieve for the Möbius function up to MU_MAX */
    static uint8_t is_comp[MU_MAX + 1];
    static uint32_t primelist[MU_MAX / 2];
    size_t pc = 0;
    mu[1] = 1;
    for (size_t i = 2; i <= MU_MAX; ++i) {
        if (!is_comp[i]) {
            primelist[pc++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t pj = 0; pj < pc; ++pj) {
            uint32_t p = primelist[pj];
            uint64_t prod = (uint64_t)i * p;
            if (prod > MU_MAX)
                break;
            is_comp[prod] = 1;
            if (i % p == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[i];
            }
        }
    }
    mu_count = MU_MAX;
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Count of square‑free integers ≤ X                                 */
/* ------------------------------------------------------------------ */
static seq_t sqfree_upto(idx_t X) {
    uint32_t x = (uint32_t)X;
    uint32_t lim = (uint32_t) sqrt((double) x);  /* integer sqrt is safe */
    seq_t sum = 0;
    for (uint32_t k = 1; k <= lim; ++k) {
        int8_t mu_k = mu[k];
        if (mu_k != 0)
            sum += (seq_t)mu_k * (seq_t)(x / (k * k));
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_tables();

    /* prime(n) is stored at index n-1 (offset 1) */
    uint32_t p_n   = primes[(size_t)(n - 1)];
    uint32_t p_np1 = primes[(size_t)n];

    seq_t sf_before   = sqfree_upto(p_n);
    seq_t sf_after    = sqfree_upto(p_np1);

    return sf_after - sf_before - (seq_t)1;
}
