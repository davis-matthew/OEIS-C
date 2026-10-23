/*
 * A171908  a(n)= Number of 5 X 5 permutation matrices with trace n and
 *          determinant -1.
 * https://oeis.org/A171908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of 5x5 permutation matrices with trace n and
 *            determinant -1 * (computed by enumerating all 5! permutations
 *            and counting those with trace n and odd sign)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t _values[6];
static bool _values_computed = false;

static void _enumerate_rec(int depth, int perm[5], bool used[5])
{
    if (depth == 5) {
        int trace = 0;
        for (int i = 0; i < 5; ++i) {
            if (perm[i] == i) ++trace;
        }
        bool visited[5] = { false };
        int cycles = 0;
        for (int i = 0; i < 5; ++i) {
            if (!visited[i]) {
                ++cycles;
                int j = i;
                do {
                    visited[j] = true;
                    j = perm[j];
                } while (j != i);
            }
        }
        int sign = ((5 - cycles) & 1) ? -1 : 1;
        if (sign == -1) {
            ++_values[trace];
        }
        return;
    }
    for (int v = 0; v < 5; ++v) {
        if (!used[v]) {
            used[v] = true;
            perm[depth] = v;
            _enumerate_rec(depth + 1, perm, used);
            used[v] = false;
        }
    }
}

static void _compute_values(void)
{
    for (int i = 0; i < 6; ++i) {
        _values[i] = 0;
    }
    int perm[5];
    bool used[5] = { false };
    _enumerate_rec(0, perm, used);
}

seq_t a(idx_t n)
{
    if (n > 5) return 0;
    if (!_values_computed) {
        _compute_values();
        _values_computed = true;
    }
    return _values[(size_t)n];
}
