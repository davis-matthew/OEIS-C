/*
 * A156320  List of prime pairs of the form (p, p+8).
 * https://oeis.org/A156320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156320
   Prime pairs of the form (p, p+8). The sequence lists the lower
   and upper primes alternately: a(1)=3, a(2)=11, a(3)=5, a(4)=13, …   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global state ------------------------------------------------------ */

static bool *is_prime = NULL;   /* sieve bitmap (size: sieve_limit+1) */
static size_t sieve_limit = 0;  /* highest number currently sieved */

static seq_t *seq = NULL;       /* array holding the sequence values */
static size_t seq_len = 0;      /* number of values stored */
static size_t seq_cap = 0;      /* capacity of seq array */

static idx_t current = 3;       /* next integer to test as the lower prime */

/* --- helpers ----------------------------------------------------------- */

static void init_sieve(void)
{
    if (is_prime != NULL)
        return;                /* already initialised */

    /* 10 000 000 is ample for the first 100 000 terms (≈70 000 pairs) */
    const size_t LIMIT = 10000000ULL;
    sieve_limit = LIMIT + 8;                /* need to test p+8 */

    is_prime = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_prime)
        abort();

    for (size_t i = 0; i <= sieve_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_prime[i])
            continue;
        for (size_t j = i * i; j <= sieve_limit; j += i)
            is_prime[j] = false;
    }

    /* Allocate space for the sequence itself. */
    seq_cap = 1024;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq)
        abort();
}

static void grow_seq(void)
{
    seq_cap *= 2;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    if (!seq)
        abort();
}

static void push_value(seq_t v)
{
    if (seq_len == seq_cap)
        grow_seq();
    seq[seq_len++] = v;
}

/* Ensure we have at least n entries in the sequence. */
static void generate_until(idx_t n)
{
    while (seq_len < n) {
        /* If we have exhausted the current sieve range, double it and
           rebuild the sieve.  This rarely triggers for the required
           limits (n <= 10 000). */
        if ((size_t)current > sieve_limit - 8) {
            size_t old_limit = sieve_limit;
            size_t new_limit = old_limit * 2;

            /* Reallocate the sieve bitmap. */
            is_prime = (bool *)realloc(is_prime, new_limit + 1);
            if (!is_prime)
                abort();

            sieve_limit = new_limit + 8;
            for (size_t i = 0; i <= sieve_limit; ++i)
                is_prime[i] = true;
            is_prime[0] = is_prime[1] = false;

            for (size_t i = 2; i * i <= sieve_limit; ++i) {
                if (!is_prime[i])
                    continue;
                for (size_t j = i * i; j <= sieve_limit; j += i)
                    is_prime[j] = false;
            }
        }

        if (is_prime[(size_t)current] &&
            is_prime[(size_t)(current + 8)]) {
            /* Store the pair in interleaved fashion. */
            push_value((seq_t)current);                /* lower prime   */
            push_value((seq_t)(current + 8));          /* upper prime   */
        }
        ++current;
    }
}

/* --------------------------------------------------- public function ---- */

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 0 */
        return 0;

    init_sieve();

    if ((size_t)seq_len < (size_t)n)
        generate_until(n);

    return seq[n - 1];
}
