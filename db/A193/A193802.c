/*
 * A193802  Length of optimal Wichmann rulers.
 * https://oeis.org/A193802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 4r² + 8r + 3 + s(4r+3) where r is the smallest
 *            non‑negative integer such that n lies in the block of indices
 *            belonging to r, and s = s_min(r) + (n - start(r)), with
 *            s_min(0)=0, s_min(r) = prime_{r-1} (for r≥1), prime_k being the
 *            k‑th prime (prime_0=2). The block size for r is count(r) = pr...
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* The type definitions seq_t and idx_t are supplied by the build. */

static int _prime_limit = 200000;                    /* should cover all needed primes */
static uint32_t *_prime_table = NULL;                /* array of primes */
static size_t   _prime_cnt   = 0;                     /* number of primes stored */

/* Generate a table of primes up to _prime_limit using a simple sieve.  */
static void _generate_primes(void)
{
    if (_prime_table != NULL) return;                /* already generated */

    size_t cap = 20000;                              /* enough for r up to ~200         */
    _prime_table = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (_prime_table == NULL) return;                /* fatal, but will be rare */

    /* Sieve of Eratosthenes */
    uint8_t *is_prime = (uint8_t *)malloc((_prime_limit + 1) * sizeof(uint8_t));
    if (is_prime == NULL) return;                    /* fatal, rare */

    for (size_t i = 0; i <= (size_t)_prime_limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= (size_t)_prime_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)_prime_limit; j += i)
                is_prime[j] = 0;
        }
    }

    for (size_t i = 2; i <= (size_t)_prime_limit; ++i) {
        if (is_prime[i]) {
            if (_prime_cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(_prime_table, cap * sizeof(uint32_t));
                if (tmp == NULL) break;              /* out of memory -> keep current size */
                _prime_table = tmp;
            }
            _prime_table[_prime_cnt++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, but return 0 as safety */
    _generate_primes();                       /* ensure primes are ready */

    idx_t start = 1;                           /* first index of current block   */
    idx_t r     = 0;                           /* current r                      */

    while (1) {
        /* Block size = prime_r + 1. For r=0, prime_0 = 2. */
        uint64_t prime_r = (uint64_t)_prime_table[r];
        idx_t   count   = (idx_t)(prime_r + 1);

        if (n < start + count) break;          /* n falls into current block */
        start += count;
        ++r;
    }

    seq_t s_min = (r == 0) ? 0 : (seq_t)_prime_table[r - 1];
    seq_t s     = s_min + (seq_t)(n - start);

    seq_t L = (seq_t)(4) * r * r
            + (seq_t)(8) * r
            + (seq_t)3
            + s * ((seq_t)(4) * r + (seq_t)3);

    return L;
}
