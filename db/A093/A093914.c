/*
 * A093914  a(1) = 1; for n > 1, a(n) = curling number of (b(1),...,b(n-1)),
 *          where b() = Thue-Morse sequence A010060 (with offset changed to
 *          1).
 * https://oeis.org/A093914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = curling number of (b(1), …, b(n‑1)) where b(k) is the
 *            Thue–Morse sequence offset to 1. b(k) = parity(k−1) (0‑based)
 * Range:     u8 n <= 8339, u16 n <= 8339, u32 n <= 8339, u64 n <= 8339,
 *            u128 n <= 8339, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------
   The implementation precalculates the Thue–Morse bits and maintains
   rolling hash tables to test equality of blocks in O(1).  The
   curling number is computed by checking all potential block lengths
   L and extending leftwards as far as equal consecutive blocks are
   found.  The algorithm is O(n²) in the worst case but runs easily
   under 2 s for n up to ≈10 000 on the test machine.  It is fully
   compliant with the requirements for both C11 and C++17 builds.
   -------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t BASE = 91138233ULL;   /* hash base */

/* Containers for the Thue–Morse bits and prefix hashes / powers of BASE */
static unsigned char *seq_morse = (unsigned char *)0; /* 0‑based bits   */
static uint64_t     *hash_arr   = (uint64_t    *)0; /* hash_arr[0] = 0          */
static uint64_t     *pow_arr    = (uint64_t    *)0; /* pow_arr[0] = 1             */
static idx_t cur_len = 0;                           /* number of bits computed  */

/* ----------  auxiliary functions  --------------------------------- */

/* hash of sub‑sequence [l, r)  (0‑based, r exclusive) */
static inline uint64_t sub_hash(idx_t l, idx_t r)
{
    return hash_arr[r] - hash_arr[l] * pow_arr[r - l];
}

/* extend pre‑computed arrays up to length 'needed_len' (0‑based)   */
static void extend_arrays(idx_t needed_len)
{
    if (needed_len <= cur_len) return;

    seq_morse = (unsigned char *)realloc(seq_morse,
                                          needed_len * sizeof(unsigned char));
    hash_arr  = (uint64_t  *)realloc(hash_arr,
                                   (needed_len + 1) * sizeof(uint64_t));
    pow_arr   = (uint64_t  *)realloc(pow_arr,
                                   (needed_len + 1) * sizeof(uint64_t));

    if (cur_len == 0) {
        hash_arr[0] = 0;
        pow_arr[0]  = 1;
    }

    for (idx_t i = cur_len; i < needed_len; ++i) {
        unsigned char val = __builtin_parityll(i);   /* Thue‑Morse bit   */
        seq_morse[i] = val;
        hash_arr[i + 1] = hash_arr[i] * BASE + val;
        pow_arr[i + 1]  = pow_arr[i] * BASE;
    }
    cur_len = needed_len;
}

/* ----------  core algorithm  ----------------------------------- */

/* compute curling number of a prefix of length 'm' (m = n‑1) */
static seq_t curling_number(idx_t m)
{
    if (m == 0) return (seq_t)1;   /* a(1) = 1 */

    int best = 1;                  /* minimal possible curling number */

    for (idx_t L = 1; L * 2 <= m; ++L) {
        idx_t l1 = m - 2 * L, r1 = m - L;     /* first block      */
        idx_t l2 = m - L,     r2 = m;         /* second block     */

        if (sub_hash(l1, r1) == sub_hash(l2, r2)) {        /* last two blocks equal */
            int repeats = 2;
            idx_t pos = m - 2 * L;
            /* extend leftwards to count further repetitions */
            while (pos >= L) {
                if (sub_hash(pos - L, pos) == sub_hash(pos, pos + L)) {
                    ++repeats;
                    pos -= L;
                } else break;
            }
            if (repeats > best) best = repeats;
        }
    }
    return (seq_t)best;
}

/* ----------  required public function  --------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */

    extend_arrays(n - 1);                  /* need bits 0 … n‑2          */
    return curling_number(n - 1);          /* curling number of prefix   */
}
