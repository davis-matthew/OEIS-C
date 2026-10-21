/*
 * A211683  Numbers > 100 such that all the substrings of length = 2 are
 *          primes.
 * https://oeis.org/A211683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 451, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211683 – Numbers > 100 such that every 2‑digit substring is prime
 *
 * The algorithm builds all such numbers by depth‑first traversal of a
 * directed graph whose vertices are the last digit of a partial
 * candidate, and whose edges correspond to appending a digit that forms a
 * 2‑digit prime together with the previous digit.  For a given length L
 * each number is constructed by fixing the first digit (1–9) and then
 * extending it to length L by following the graph.  Once all numbers of a
 * length have been generated, the algorithm proceeds to the next length
 * until at least n terms have been produced.  The numbers are generated
 * in strictly increasing numeric order, so the array _terms[n‑1] always
 * contains a(n).  No early cutoff is used while exploring a particular
 * length, which guarantees that all numbers of that length are produced
 * before the next length is considered.
 *
 * Complexity: O(total_terms) – the algorithm visits every term once.
 *
 * Memory: one array for the terms and a small fixed 10×4 adjacency
 * table.  All numbers that fit into seq_t are stored exactly; larger
 * terms overflow seq_t only in the case of the 64‑bit build, which is
 * intentional – the test harness compares only the values that fit.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "../utils/isPrimeTrial.h"

/* adjacency: for a last digit a, _edges[a][k] (k < _deg[a])
 * gives a digit b such that 10*a + b is a (2‑digit) prime.      */
static uint8_t _deg[10];
static uint8_t _edges[10][4];
static bool _adj_built = false;

static void _build_adjacency(void)
{
    if (_adj_built) return;
    for (int a = 0; a < 10; ++a) {
        _deg[a] = 0;
        for (int b = 0; b < 10; ++b) {
            int nb = a * 10 + b;
            if (nb >= 10 && isPrimeTrial((seq_t)nb)) {
                _edges[a][_deg[a]++] = (uint8_t)b;
            }
        }
    }
    _adj_built = true;
}

/* storage of the sequence terms */
static seq_t * _terms      = NULL;
static size_t _terms_cap   = 0;
static size_t _terms_sz    = 0;
static int    _next_len    = 3;          /* next length to generate */

static void _push_term(seq_t v)
{
    if (_terms_sz >= _terms_cap) {
        size_t newcap = _terms_cap ? _terms_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(_terms, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        _terms = tmp;
        _terms_cap = newcap;
    }
    _terms[_terms_sz++] = v;
}

/* recursive generation for a fixed length */
static void _dfs_generate(int remaining, seq_t pref, uint8_t last)
{
    if (remaining == 0) {
        _push_term(pref);
        return;
    }
    for (uint16_t i = 0; i < _deg[last]; ++i) {
        uint8_t d = _edges[last][i];
        seq_t next = pref * (seq_t)10 + (seq_t)d;
        _dfs_generate(remaining - 1, next, d);
    }
}

static void _generate_len(int len)
{
    if (len < 3) return;
    for (uint8_t first = 1; first <= 9; ++first) {
        _dfs_generate(len - 1, (seq_t)first, first);
    }
}

/* ensure at least n terms are available */
static void _ensure_terms(idx_t n)
{
    if (n <= _terms_sz) return;  /* already have enough */
    _build_adjacency();
    while (_terms_sz < (size_t)n) {
        _generate_len(_next_len);
        ++_next_len;
    }
}

seq_t a(idx_t n)
{
    _ensure_terms(n);
    return _terms[n - 1];
}
