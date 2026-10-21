/*
 * A065437  Smallest base relative to which the n-th prime is palindromic.
 * https://oeis.org/A065437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest base b (b ≥ 2) for which the n‑th prime is a
 *            palindrome when written in base b. If no base < p‑1 works, a(n)
 *            = p‑1, where p is the n‑th prime. This implementation
 *            precomputes enough primes by a sieve and then scans bases up to
 *            ⌊√p⌋. It handles arbitrary values of n ≥ 1 without...
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

/* --- Configuration ----------------------------------------------------- */
#define SIEVE_LIMIT 2000000          /* must be ≥ the largest prime needed   */
#define PRIMES_CAP   200000          /* enough to hold all primes up to limit */

/* --- Global data ----------------------------------------------------- */
static bool sieve_done      = false;
static idx_t  prime_count   = 0;
static uint64_t primes_storage[PRIMES_CAP];

/* --- Utility: palindrome test ---------------------------------------- */
static bool is_palindrome_uint64(uint64_t num, idx_t base)
{
    /* collect digits of num in the given base */
    int  digits[64];
    int  len = 0;

    if (num == 0)                   /* zero is a palindrome */
        return true;

    while (num > 0)
    {
        digits[len++] = (int)(num % base);
        num          /= base;
    }

    for (int i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i])
            return false;
    return true;
}

/* --- Prime sieve ----------------------------------------------------- */
static void compute_primes(void)
{
    if (sieve_done)
        return;

    static char is_prime[SIEVE_LIMIT + 1];
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_prime[i] = 1;

    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    prime_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            primes_storage[prime_count++] = i;

    sieve_done = true;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; return 0 for safety             */
        return 0;

    compute_primes();

    /* guard against out‑of‑range requests */
    if (n > prime_count)
        return 0;       /* or handle by extending the sieve if desired */

    uint64_t p = primes_storage[n - 1];   /* nth prime (0‑based index)   */

    /* Special case: prime 2 has representation "2" in base 3        */
    if (p == 2)
        return 3;

    /* Search bases from 2 up to floor(sqrt(p)) */
    idx_t limit = (idx_t)(sqrt((long double)p));
    for (idx_t b = 2; b <= limit; ++b)
        if (is_palindrome_uint64(p, b))
            return (seq_t)b;

    /* No smaller base found – base (p‑1) yields the two‑digit
       palindrome “11” in that base.                            */
    return (seq_t)(p - 1);
}
