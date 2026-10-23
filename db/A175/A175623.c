/*
 * A175623  Triangle read by rows: T(n,k) = prime(k)^(n-1) mod n, 1<=k<=n.
 * https://oeis.org/A175623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = prime(k)^(r-1) mod r, where r is the smallest integer
 *            such that r*(r+1)/2 >= n and k = n - r*(r-1)/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PRIMES   50000
#define PRIME_LIMIT  600000   /* enough to generate at least MAX_PRIMES primes */

static uint64_t primes[MAX_PRIMES];
static size_t  prime_count   = 0;
static bool    primes_ready  = false;

/* Generate a table of primes up to PRIME_LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (primes_ready)
        return;

    static unsigned char sieve[PRIME_LIMIT + 1] = {0};
    size_t count = 0;

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            if (count < MAX_PRIMES)
                primes[count++] = (uint64_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    prime_count = count;
    primes_ready = true;
}

/* Modular exponentiation: compute (base^exp) mod mod.  All arithmetic
   performed in seq_t; intermediate multiplication uses uint64_t to avoid
   overflow. */
static seq_t modpow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b      = base % mod;
    uint64_t m   = (uint64_t)mod;

    while (exp > 0) {
        if (exp & 1)
            result = (seq_t)((((uint64_t)result * (uint64_t)b) % m));
        b = (seq_t)(((uint64_t)b * (uint64_t)b) % m);
        exp >>= 1;
    }
    return result;
}

/* Find the 1‑based row index r for a given term position n. */
static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    idx_t cum = 1;            /* cumulative count r(r+1)/2 */
    while (cum < n) {
        ++r;
        cum += r;
    }
    return r;
}

/* OEIS sequence A175623: value of the nth entry of the triangle read by rows. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1 */
        return 0;

    init_primes();

    /* Determine row r and column k. */
    idx_t r = find_row(n);
    idx_t prev_sum = r * (r - 1) / 2;   /* r(r-1)/2 fits in idx_t */
    idx_t k = n - prev_sum;             /* 1‑based column index, 1 ≤ k ≤ r */

    /* Fetch the k‑th prime (1‑based). */
    if ((size_t)k > prime_count)
        return 0;                       /* safety guard, should not happen with limits */

    seq_t prime_k = (seq_t)primes[k - 1];

    /* Compute prime_k^(r-1) modulo r. */
    seq_t mod = (seq_t)r;
    return modpow(prime_k, (uint64_t)(r - 1), mod);
}
