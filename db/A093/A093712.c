/*
 * A093712  Repeatedly subtract largest prime from n until either a prime or 1
 *          remains.
 * https://oeis.org/A093712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = decimal concatenation of the successive subtractions of
 *            the largest prime ≤ current until a prime or 1 remains. a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=31, a(5)=5, a(6)=51, a(7)=7, a(8)=71,
 *            a(9)=72, a(10)=73, … (see OEIS A093712)
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Cache of the primality table up to the largest n requested. */
static idx_t prime_limit = 0;
static bool *prime_flag  = NULL;

/* Ensure that prime_flag[0…limit] is set; true iff the index is prime. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;           /* already have enough */

    /* free the old table if present */
    if (prime_flag != NULL) {
        free(prime_flag);
        prime_flag = NULL;
    }

    /* allocate a new table (size limit + 1) */
    prime_flag = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (prime_flag == NULL)
        exit(1);           /* out of memory */

    /* trivial primes */
    for (idx_t i = 2; i <= limit; ++i)
        prime_flag[i] = true;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = false;
        }
    }
    prime_limit = limit;
}

/* Return the largest prime ≤ x (x ≥ 2). */
static idx_t
largest_prime_le(idx_t x)
{
    /* Search backward until a marked prime is found. */
    for (idx_t i = x; i >= 2; --i)
        if (prime_flag[i])
            return i;
    return 2;   /* safety: will never happen for x ≥ 2 */
}

/* Number of decimal digits of a non‑negative idx_t value. */
static int
num_digits_idx(idx_t v)
{
    if (v == 0)
        return 1;
    int d = 0;
    for (idx_t tmp = v; tmp > 0; tmp /= 10)
        ++d;
    return d;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)         /* the sequence is defined for n ≥ 1 */
        return 0;

    /* Make sure we can test primality up to n and later values. */
    ensure_primes(n);

    idx_t current = n;
    seq_t result  = 0;

    for (;;) {
        /* Stop when the current value is 1 or prime. */
        if (current == 1 || prime_flag[current]) {
            int d = num_digits_idx(current);
            seq_t mult = 1;
            for (int i = 0; i < d; ++i)
                mult *= (seq_t)10;
            result = result * mult + (seq_t)current;
            break;
        }

        /* Subtract the largest prime ≤ current. */
        idx_t p = largest_prime_le(current);
        idx_t new_current = current - p;

        int d = num_digits_idx(p);
        seq_t mult = 1;
        for (int i = 0; i < d; ++i)
            mult *= (seq_t)10;
        result = result * mult + (seq_t)p;

        current = new_current;
    }

    return result;
}
