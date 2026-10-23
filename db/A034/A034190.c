/*
 * A034190  Number of binary codes of length 5 with n words.
 * https://oeis.org/A034190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   permute_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A034190 – Number of binary codes of length 5 with n words.
   The number equals the number of n‑subsets of the 32‑vertex hypercube Q₅ that
   are closed under its full automorphism group.  With |G| = 5!·2⁵ = 3840,
   Polya’s enumeration theorem gives
        a(n) = (1/3840) * Σ_{g∈G} f_g(n)
   where f_g(n) counts the n‑subsets fixed by the group element g.  For an
   element g=(perm,flip) the induced action on a vertex v is
        v' = permute_bits(perm, v) XOR flip.
   A subset is fixed iff it is a union of full cycles of this permutation;
   thus f_g(n) is the number of ways to choose cycles whose lengths sum to n.
   Since the hypercube has exactly 32 vertices, a(n)=0 for n > 32.  The
   implementation below pre‑computes a(0)…a(32) once (O(3840·32) time) and
   returns the cached value for any subsequent call.  It is fully
   compatible with C11 and C++17 and works for arbitrary‑precision seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/permute_bits.h"

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);

/* Number of elements in the automorphism group of Q₅. */
#define GROUP_SIZE 3840

/* Cache for values 0 … 32.  The table is computed lazily. */
static seq_t table_a[33];
static bool table_computed = false;

/* Return true if the array ``a`` of length ``len`` was changed to its next
   lexicographic permutation.  False means it was already the last (descending). */
static bool next_permutation(int *a, size_t len)
{
    if (len < 2) return false;
    /* step 1: find largest i with a[i] < a[i+1] */
    size_t i = len - 1;
    while (i > 0 && a[i - 1] >= a[i]) --i;
    if (i == 0) return false;
    --i;
    /* step 2: find largest j > i with a[j] > a[i] */
    size_t j = len - 1;
    while (a[j] <= a[i]) --j;
    /* step 3: swap a[i] and a[j] */
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    /* step 4: reverse a[i+1 .. end] */
    ++i;
    size_t end = len;
    while (i < end - 1) {
        tmp = a[i];
        a[i] = a[end - 1];
        a[end - 1] = tmp;
        ++i;
        --end;
    }
    return true;
}

/* Pre‑compute table_a[0 .. 32].  The table is populated only once. */
static void precompute_table(void)
{
    seq_t sum[33] = {0};

    int perm[5] = {0, 1, 2, 3, 4};
    unsigned map[32];
    bool seen[32];
    unsigned lengths[32];
    seq_t ways[33];
    unsigned clen;

    do {
        /* Enumerate all 32 possible bit flips. */
        for (unsigned flip = 0; flip < 32; ++flip) {
            /* Build the mapping induced by the current (perm,flip). */
            for (unsigned v = 0; v < 32; ++v) {
                unsigned u;
                permute_bits(perm, v, &u);
                map[v] = u ^ flip;
            }

            /* Decompose the mapping into disjoint cycles. */
            for (int i = 0; i < 32; ++i) seen[i] = false;
            clen = 0;
            for (unsigned v = 0; v < 32; ++v) {
                if (!seen[v]) {
                    unsigned cur = v, len = 0;
                    do {
                        seen[cur] = true;
                        cur = map[cur];
                        ++len;
                    } while (cur != v);
                    lengths[clen++] = len;
                }
            }

            /* Compute ways[s] = number of cycle selections summing to s. */
            for (int s = 0; s <= 32; ++s) ways[s] = 0;
            ways[0] = 1;
            for (unsigned i = 0; i < clen; ++i) {
                unsigned l = lengths[i];
                for (int s = 32; s >= (int)l; --s)
                    ways[s] += ways[s - l];
            }

            /* Accumulate into the global sums. */
            for (unsigned n = 0; n <= 32; ++n)
                sum[n] += ways[n];
        }
    } while (next_permutation(perm, 5));

    /* Divide by |G| to get the final sequence values. */
    for (unsigned n = 0; n <= 32; ++n)
        table_a[n] = sum[n] / GROUP_SIZE;

    table_computed = true;
}

seq_t a(idx_t n)
{
    if (!table_computed)
        precompute_table();

    if (n <= 32u)
        return table_a[(size_t)n];
    else
        return (seq_t)0;
}
