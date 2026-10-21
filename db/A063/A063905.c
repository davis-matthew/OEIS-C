/*
 * A063905  Each prime p appears p times.
 * https://oeis.org/A063905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the k‑th prime p_k where sum_{i=1}^{k-1} p_i < n ≤
 *            sum_{i=1}^{k} p_i. In other words, each prime p appears exactly
 *            p consecutive terms in the sequence
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function assumes seq_t and idx_t are defined elsewhere
   (e.g. seq_t = uint32_t, idx_t = uint64_t). */

#include <stdlib.h>
#include <stdbool.h>

/* Simple primality test suitable for the small numbers needed
   in this sequence.  It is pure integer arithmetic, no math.h. */
static bool is_prime(idx_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0)
        return false;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Static tables of primes and their cumulative sums. */
    static idx_t *primes  = NULL;   /* primes[0..size-1] */
    static idx_t *cumsum = NULL;   /* cumulative sums of primes */
    static size_t  size   = 0;      /* number of primes stored */
    static size_t  cap    = 0;      /* allocated capacity */

    /* Grow tables until the cumulative sum reaches or exceeds n. */
    while (size == 0 || cumsum[size - 1] < n)
    {
        /* Find the next prime after the last stored one. */
        idx_t candidate = (size == 0) ? 2 : primes[size - 1] + 1;
        while (!is_prime(candidate))
            ++candidate;

        /* Resize if necessary. */
        if (size >= cap)
        {
            cap = (cap == 0) ? 4 : cap * 2;
            primes  = (idx_t *)realloc(primes,  cap * sizeof(idx_t));
            cumsum = (idx_t *)realloc(cumsum, cap * sizeof(idx_t));
        }

        /* Store the new prime and update cumulative sum. */
        primes[size] = candidate;
        if (size == 0)
            cumsum[size] = candidate;
        else
            cumsum[size] = cumsum[size - 1] + candidate;
        ++size;
    }

    /* Binary search for the first prime whose cumulative sum ≥ n. */
    size_t lo = 0, hi = size - 1, mid, idx = size - 1;
    while (lo <= hi)
    {
        mid = lo + (hi - lo) / 2;
        if (cumsum[mid] >= n) {
            idx = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    return (seq_t)primes[idx];
}
