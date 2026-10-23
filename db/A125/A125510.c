/*
 * A125510  Theta series of 4-dimensional lattice QQF.4.g.
 * https://oeis.org/A125510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * b(n) where b(n) is multiplicative: b(2^e) = 1, b(3^e)
 *            = 3^(e+1) - 2, b(p^e) = (p^(e+1) - 1) / (p - 1) for p > 3 a(0) =
 *            1 The function factors n and evaluates the product of local
 *            terms
 * Range:     u8 n <= 16, u16 n <= 1995, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Build an auxiliary prime list once.  A small limit suffices for the
 * typical n values (≤ 10000) but we use a generous bound to keep the
 * function correct for larger arguments.  The list is re‑used for
 * successive calls. */
#define PRIME_SIEVE_LIMIT 100000

static uint32_t *prime_list = NULL;
static size_t prime_count   = 0;

/* Simple sieve of Eratosthenes, executed lazily. */
static void init_prime_list(void)
{
    if (prime_list != NULL) return;          /* already built */

    bool *is_prime = (bool *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_prime) return;                   /* allocation failure → silently skip */

    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes to allocate the exact array size. */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(is_prime); return; }

    prime_count = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    free(is_prime);
}

/* Fast exponentiation for seq_t using only *, - and / operators.
 * For generic big‑integer types this works because those operators are
 * overloaded.  The exponent is at most log₂(n), hence small. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) { result *= base; --exp; }
    return result;
}

/* Compute the multiplicative function b(n). */
static seq_t comp_b(uint64_t n)
{
    if (n <= 1) return (seq_t)1;          /* b(1) = 1 */

    init_prime_list();

    seq_t product = (seq_t)1;
    uint64_t m = n;

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }

            seq_t term;
            if (p == 2) {
                term = (seq_t)1;
            } else if (p == 3) {
                term = pow_seq((seq_t)3, (idx_t)(e + 1)) - (seq_t)2;
            } else {
                seq_t num = pow_seq((seq_t)p, (idx_t)(e + 1)) - (seq_t)1;
                seq_t denom = (seq_t)(p - 1);
                term = num / denom;            /* exact division */
            }
            product *= term;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1) {
        uint32_t p = (uint32_t)m;
        if (p == 2) {
            /* term = 1 → nothing changes */
        } else if (p == 3) {
            product *= (pow_seq((seq_t)3, (idx_t)2) - (seq_t)2);    /* e = 1 */
        } else {
            seq_t num = pow_seq((seq_t)p, (idx_t)2) - (seq_t)1;     /* e = 1 */
            seq_t denom = (seq_t)(p - 1);
            product *= num / denom;
        }
    }

    return product;
}

/* Public function implementing A125510. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t b = comp_b((uint64_t)n);
    return (seq_t)6 * b;
}
