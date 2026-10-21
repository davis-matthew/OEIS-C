/*
 * A057541  Birthday set of order 9: i.e., numbers congruent to +- 1 modulo 2,
 *          3, 4, 5, 6, 7, 8 and 9.
 * https://oeis.org/A057541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057541: Birthday set of order 9.
   A(n) is the nth positive integer that is congruent to ±1 modulo all
   integers 2,3,…,9.
   That is, A(n) satisfies
        x ≡ 1 or -1 (mod m)  for every m = 2,…,9.
   As the modulus lcm(2,…,9) = 2520, the set of admissible residues
   modulo 2520 is finite.  The implementation pre‑computes these
   residues once and then counts how many admissible numbers are ≤ X;
   a binary search uses that count to find the nth admissible number. */

#include <stdint.h>
#include <stddef.h>

/* Max residue count (≤2520).  We allocate the full array and keep the
   actual count in residues_cnt. */
static uint64_t residues[2520];
static size_t residues_cnt = 0;
static int residues_ready = 0;

/* Build the list of residues r (0 < r < 2520) such that
   r % m == 1 or r % m == m-1 for every m = 2…9. */
static void init_residues(void)
{
    if (residues_ready) return;
    size_t cnt = 0;
    for (uint64_t r = 0; r < 2520; ++r) {
        int ok = 1;
        for (uint64_t m = 2; m <= 9; ++m) {
            uint64_t mod = r % m;
            if (mod != 1 && mod != m - 1) {
                ok = 0;
                break;
            }
        }
        if (ok && r != 0) {
            residues[cnt++] = r;
        }
    }
    residues_cnt = cnt;
    residues_ready = 1;
}

/* Count how many admissible numbers are ≤ X. */
static unsigned long long count_leq(unsigned long long X)
{
    unsigned long long total = 0;
    const unsigned long long L = 2520ULL;
    for (size_t i = 0; i < residues_cnt; ++i) {
        unsigned long long r = residues[i];
        if (X < r) continue;
        total += (X - r) / L + 1ULL;
    }
    return total;
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Prepare residues once. */
    init_residues();

    /* n is 1‑based */
    unsigned long long target = (unsigned long long)n;

    /* Binary search bounds.  Upper bound is increased until it
       surely contains the nth term. */
    unsigned long long low = 1ULL;
    unsigned long long high = target * 2520ULL;
    if (high == 0ULL) high = 1ULL;

    while (count_leq(high) < target) {
        high *= 2ULL;
    }

    /* Binary search for smallest X with count_leq(X) ≥ target. */
    while (low < high) {
        unsigned long long mid = low + (high - low) / 2ULL;
        if (count_leq(mid) >= target)
            high = mid;
        else
            low = mid + 1ULL;
    }
    return (seq_t)low;
}
