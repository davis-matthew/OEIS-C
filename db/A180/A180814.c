/*
 * A180814  Number of distinct solutions of Sum_{i=1..2}(x(2i-1)*x(2i)) = 0
 *          (mod n), with x() only in 2..n-2.
 * https://oeis.org/A180814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 71, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of distinct quadruples (x1,x2,x3,x4) with
 *  2 <= xi <= n-2 such that x1*x2 + x3*x4 ≡ 0 (mod n).
 *  Each pair of numbers is unordered and the two products are
 *  unordered (only the multiset of two products matters). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cnt = NULL;
static size_t cnt_size = 0;

seq_t a(idx_t n)
{
    /* For n < 4 there are at most two admissible numbers.  The
     * following generic algorithm already yields the correct
     * result for all n. */

    if (n < 4) {
        /* For n = 1,2,3 the admissible range [2,n-2] is empty. */
        return (seq_t)0;
    }

    /* Range of admissible values: [2, n-2] */
    uint32_t low  = 2;
    uint32_t high = (uint32_t)n - 2;
    uint32_t limit = (uint32_t)n;                /* modulus size */

    /* Allocate or resize the array that holds counts of unordered
     * pairs for each residue modulo n. */
    if (cnt == NULL || cnt_size < limit) {
        free(cnt);
        cnt = (seq_t *)malloc(limit * sizeof(seq_t));
        if (!cnt) return (seq_t)0;   /* allocation failed */
        cnt_size = limit;
    }
    /* clear counts */
    for (size_t i = 0; i < limit; ++i) cnt[i] = 0;

    /* Count unordered pairs (a,b) with a <= b. */
    for (uint32_t a = low; a <= high; ++a) {
        uint64_t a_val = a;
        for (uint32_t b = a; b <= high; ++b) {
            uint64_t prod = a_val * (uint64_t)b;
            uint32_t r = (uint32_t)(prod % limit);
            cnt[r]++;                     /* count this unordered pair */
        }
    }

    /* Compute the number of unordered solutions.  For each residue
     * r we need to combine pairs with residue r and with residue
     * (-r mod n).  If r == -r we use combinations WITH
     * replacement:  C*(C+1)/2.  Otherwise we pair each element of
     * residue r with each element of residue -r, counting each
     * unordered pair only once (hence the r < s test). */
    seq_t total = 0;
    for (size_t r = 0; r < limit; ++r) {
        seq_t cr = cnt[r];
        if (cr == 0) continue;

        size_t s = (r == 0) ? 0 : limit - r;   /* (-r mod n) */
        if (s == r) {                /* 2r ≡ 0 (mod n) */
            total += cr * (cr + 1) / 2;
        } else if (r < s) {
            total += cr * cnt[s];
        }
    }
    return total;
}
