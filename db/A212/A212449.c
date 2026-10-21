/*
 * A212449  Floor(5n + log(5n)).
 * https://oeis.org/A212449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n + log(5*n))
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1.  For n = 0 we return 0
   just to maintain a valid (unused) value.  log is the natural
   logarithm.  The implementation uses a small table of the
   integer ceilings of eⁿ to determine floor(log x) without
   any floating‑point arithmetic. */

#include <stdint.h>          /* for uint64_t */

seq_t a(idx_t n)
{
    /* a(0) is outside the OEIS offset – return 0 to avoid
       overflow or use of uninitialized memory. */
    if (n == 0)
        return 0;

    /* Compute x = 5*n.  The result fits in seq_t for all
       n that satisfy the required limit (e.g. n ≤ 10 000). */
    seq_t x = (seq_t)5 * n;

    /* Ceil(e^k) for k = 0 … 20.  These are pre‑computed
       integer thresholds: if ceil(e^k) ≤ x < ceil(e^(k+1))
       then floor(log x) = k.  The largest k needed for
       5*n ≤ 50 000 is 20. */
    static const uint64_t ceil_e_powers[] = {
        1ULL,          /* ceil(e^0) = 1          */
        3ULL,          /* ceil(e^1) = 3          */
        8ULL,          /* ceil(e^2) = 8          */
        21ULL,         /* ceil(e^3) = 21         */
        55ULL,         /* ceil(e^4) = 55         */
        149ULL,        /* ceil(e^5) = 149        */
        404ULL,        /* ceil(e^6) = 404        */
        1097ULL,       /* ceil(e^7) = 1097       */
        2981ULL,       /* ceil(e^8) = 2981       */
        8104ULL,       /* ceil(e^9) = 8104       */
        22027ULL,      /* ceil(e^10) = 22027     */
        59875ULL,      /* ceil(e^11) = 59875     */
        162755ULL,     /* ceil(e^12) = 162755    */
        442414ULL,     /* ceil(e^13) = 442414    */
        1202605ULL,    /* ceil(e^14) = 1202605   */
        3269018ULL,    /* ceil(e^15) = 3269018   */
        8875826ULL,    /* ceil(e^16) = 8875826   */
        24154953ULL,   /* ceil(e^17) = 24154953  */
        65834171ULL,   /* ceil(e^18) = 65834171  */
        179482712ULL,  /* ceil(e^19) = 179482712 */
        485165196ULL   /* ceil(e^20) = 485165196 */
    };
    static const idx_t table_size = 21;

    /* Convert x to a 64‑bit unsigned integer for comparison.
       The value 5*n is at most 5 000 000 for n≤1 000 000, well
       below 2^64, so unsigned long long is sufficient. */
    uint64_t ux = (uint64_t)x;

    /* Find the largest k such that ceil(e^k) ≤ x.  Since
       table_size is a constant, a simple linear search is
       fast enough (≤20 iterations). */
    idx_t k = 0;
    while (k + 1 < table_size && ux >= ceil_e_powers[k + 1])
        ++k;

    /* floor(log x) == k for the found k.  The final value is
       x + k, which is equal to floor(5*n + log(5*n)). */
    return x + (seq_t)k;
}
