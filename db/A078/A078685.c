/*
 * A078685  Minimum value of |prime(n) - 2^x|.
 * https://oeis.org/A078685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A078685
 * a(n) = minimum absolute difference between the nth prime and a power of two.
 * The primes are generated incrementally; for a given prime p we locate the
 * nearest power of two using bit operations and compute the smaller
 * of the two distances.  No use of external libraries beyond <stdlib.h>
 * and <stdbool.h> is required. */

#include <stdbool.h>
#include <stdlib.h>

/*   idx_t is the index type supplied by the build (e.g., uint64_t)
 *   seq_t is the sequence value type supplied by the build
 *   (e.g., uint8_t, uint16_t, uint32_t, uint64_t, or arbitrary‑precision). */

/* ---------- Prime cache --------------------------------------- */
static idx_t *prime_array   = NULL;   /* stored primes */
static size_t prime_count   = 0;       /* number of primes stored */
static size_t prime_cap     = 0;       /* allocated capacity */
static idx_t  next_candidate = 2;     /* next integer to test */

/* Grow the prime array to hold at least “needed” entries */
static void grow_prime_array(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_array, new_cap * sizeof(idx_t));
    if (!tmp) { free(prime_array); exit(1); }
    prime_array = tmp;
    prime_cap   = new_cap;
}

/* Ensure that prime_array contains at least n primes (1‑based index). */
static void ensure_primes(idx_t n)
{
    while (prime_count < n) {
        idx_t cand = next_candidate;

        /* Prime test by trial division using stored primes.  */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_array[i];
            if (p > cand / p) break;          /* stop when p² > cand */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            grow_prime_array(prime_count + 1);
            prime_array[prime_count++] = cand;
            next_candidate = (cand == 2) ? 3 : cand + 2;
        } else {
            next_candidate = (cand == 2) ? 3 : cand + 2;
        }
    }
}

/* ---------- Sequence computation -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe if called */

    ensure_primes(n);
    seq_t prime = prime_array[n - 1];      /* nth prime (1‑based) */

    /* Find the greatest power of two <= prime.  */
    seq_t low = 1;                         /* low ≤ prime */
    while (low <= (prime >> 1)) low <<= 1; /* stop before overflow */

    /* Now prime < low<<1  (high power of two).  Compute distances.  */
    seq_t diff_low  = prime - low;                  /* ≥0 */
    seq_t diff_high = low - (prime - low);          /* (low<<1)-prime, no overflow */

    return diff_low < diff_high ? diff_low : diff_high;
}
