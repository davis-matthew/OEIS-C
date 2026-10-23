/*
 * A180826  Number of distinct solutions of sum{i=1..3}(x(2i-1)*x(2i)) = 1
 *          (mod n), with x() only in 2..n-2.
 * https://oeis.org/A180826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct triples of unordered pairs
 *            {x1,x2},{x3,x4},{x5,x6} with 2 ≤ xi ≤ n-2 such that (x1*x2 +
 *            x3*x4 + x5*x6) ≡ 1 (mod n), counted modulo permutation of the
 *            three pairs and order within each pair
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 186, u64 n <= 444,
 *            u128 n <= 444, big any n
 * Helpers:   comb2_repeat, comb3_repeat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: O(n²) time, O(n) memory. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/comb2_repeat.h"
#include "../utils/comb3_repeat.h"

seq_t a(idx_t n) {
    if (n <= 4) {
        return 0;
    }

    /* Count unordered pairs with repetition.
       Number of possible values: 2 .. n-2 */
    idx_t start = 2;
    idx_t end   = n - 2;
    idx_t len   = end - start + 1;          /* = n-3 */

    /* Frequency array: freq[p] = number of unordered pairs with product ≡ p (mod n) */
    idx_t *freq = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!freq) return 0; /* allocation failure – return 0 as safeguard */

    /* Enumerate all unordered pairs with repetition */
    for (idx_t a = start; a <= end; ++a) {
        for (idx_t b = a; b <= end; ++b) {
            idx_t prod = (a * b) % n;
            ++freq[prod];
        }
    }

    seq_t result = 0;

    /* ---------- Pattern 1: all three pairs have the same residue ----------
       Count all multisets of 3 from freq[r] unordered pairs (allowing repetitions) */
    for (idx_t r = 0; r < n; ++r) {
        if (freq[r] == 0) continue;
        if ((3 * r) % n != 1) continue;
        result += comb3_repeat(freq[r]);   /* C(freq[r]+2,3) */
    }

    /* ---------- Pattern 2: two pairs with residue a, one pair with residue b (a != b) ----------
       For each a, the required b is uniquely determined by 2*a + b ≡ 1 (mod n) */
    for (idx_t a = 0; a < n; ++a) {
        if (freq[a] == 0) continue;
        idx_t two_a_mod = (a * 2) % n;
        idx_t b = (1 + n - two_a_mod) % n;
        if (b == a) continue;              /* need distinct residues */
        if (freq[b] == 0) continue;

        seq_t ways_a = comb2_repeat(freq[a]);   /* ways to pick two from residue a (with repetition) */
        seq_t ways   = ways_a * (seq_t)freq[b];
        result += ways;
    }

    /* ---------- Pattern 3: all three pairs have distinct residues ----------
       Iterate over sorted residues a ≤ b ≤ c, enforce c > b to avoid duplicates */
    for (idx_t a = 0; a < n; ++a) {
        if (freq[a] == 0) continue;
        seq_t fa = (seq_t)freq[a];
        for (idx_t b = a + 1; b < n; ++b) {
            if (freq[b] == 0) continue;
            seq_t fb = (seq_t)freq[b];
            idx_t sum_ab = (a + b) % n;
            idx_t c = (1 + n - sum_ab) % n;
            if (c <= b) continue;               /* enforce c > b */
            if (freq[c] == 0) continue;
            seq_t fc = (seq_t)freq[c];
            result += fa * fb * fc;
        }
    }

    free(freq);
    return result;
}
