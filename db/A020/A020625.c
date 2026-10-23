/*
 * A020625  Smallest nonempty set S containing prime divisors of 9k+3 for each
 *          k in S.
 * https://oeis.org/A020625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020625: Smallest nonempty set S containing prime
   divisors of 9k+3 for each k in S. The minimal set is finite
   and consists of the primes 2,3,5,7,11,13,17. */
#include <stdint.h>
#include <stddef.h>

/* Helper to insert a prime into a sorted dynamic array,
   avoiding duplicates. */
static void add_prime(seq_t p, seq_t *arr, size_t *cnt)
{
    /* check duplicates */
    for (size_t i = 0; i < *cnt; ++i)
        if (arr[i] == p)
            return;

    /* find insertion position to keep array sorted */
    size_t pos = 0;
    while (pos < *cnt && arr[pos] < p)
        ++pos;

    /* shift elements to create space */
    for (size_t i = *cnt; i > pos; --i)
        arr[i] = arr[i - 1];

    arr[pos] = p;
    ++(*cnt);
}

seq_t a(idx_t n)
{
    static seq_t primes[128];
    static size_t cnt = 0;
    static int built = 0;

    if (!built) {
        /* start with the smallest prime */
        add_prime((seq_t)2, primes, &cnt);

        /* iterative closure under the defining relation */
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p = primes[i];
            seq_t N = (seq_t)9 * p + (seq_t)3;

            /* factor N using 64‑bit arithmetic (numbers here are tiny) */
            uint64_t val = (uint64_t)N;
            uint64_t rem = val;

            /* handle factor 2 */
            if ((rem & 1) == 0) {
                add_prime((seq_t)2, primes, &cnt);
                while ((rem & 1) == 0)
                    rem >>= 1;
            }

            /* handle odd factors */
            for (uint64_t d = 3; d * d <= rem; d += 2) {
                if (rem % d == 0) {
                    add_prime((seq_t)d, primes, &cnt);
                    while (rem % d == 0)
                        rem /= d;
                }
            }
            if (rem > 1)
                add_prime((seq_t)rem, primes, &cnt);
        }
        built = 1;
    }

    if (n == 0)
        return (seq_t)0;
    size_t idx = (size_t)(n - 1);
    if (idx < cnt)
        return primes[idx];

    /* sequence terminates after the 7 primes */
    return (seq_t)0;
}
