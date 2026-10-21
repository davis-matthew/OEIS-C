/*
 * A071578  Number of iterations of Pi(n) needed to reach 1, where Pi(x)
 *          denotes the number of primes <= x.
 * https://oeis.org/A071578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071578
   Formula: a(n) = a(Pi(n)) + 1,   a(1) = 0
   where Pi(x) counts the primes ≤ x.  The function returns the
   number of iterations of Pi needed to reach 1. */

#include <stdlib.h>
#include <string.h>

/* static state – grows the sieve and Pi array as needed */
static size_t sieve_limit = 0;                 /* largest n that has been sieved */
static uint8_t *is_composite = NULL;           /* is_composite[i] == 1 iff i is composite */
static seq_t *pi_arr      = NULL;              /* pi_arr[i] = Pi(i) for 0 ≤ i ≤ sieve_limit */

/* -------------------------------------------------------------------- */
/* Expand the sieve and Pi array up to at least n. */
static void
ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;                                 /* already available */

    size_t new_limit = (size_t)n;               /* cast to host byte size */

    /* resize/completely rebuild is_composite */
    uint8_t *tmp_comp = (uint8_t *)realloc(is_composite, new_limit + 1);
    if (!tmp_comp) abort();
    is_composite = tmp_comp;
    memset(is_composite, 0, new_limit + 1);
    /* mark 0 and 1 as composite */
    if (new_limit >= 0) is_composite[0] = 1;
    if (new_limit >= 1) is_composite[1] = 1;

    /* simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= new_limit; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                is_composite[j] = 1;

    /* resize/rebuild pi_arr */
    seq_t *tmp_pi = (seq_t *)realloc(pi_arr, (new_limit + 1) * sizeof(seq_t));
    if (!tmp_pi) abort();
    pi_arr = tmp_pi;
    pi_arr[0] = 0;
    if (new_limit >= 1) pi_arr[1] = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        pi_arr[i] = pi_arr[i - 1] + (!is_composite[i] ? 1 : 0);

    sieve_limit = new_limit;
}

/* -------------------------------------------------------------------- */
/* Return the value a(n) for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                                   /* sequence is defined for n ≥ 1 */

    ensure_sieve(n);

    seq_t steps = 0;
    /* iterate Pi until 1 is reached */
    while (n > 1) {
        n = (idx_t)pi_arr[n];
        ++steps;
    }
    return steps;
}
