/*
 * A151722  Weight distribution of [63,18,21] primitive binary BCH code.
 * https://oeis.org/A151722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A151722: weight distribution of the [63,18,21] primitive binary BCH code.
   The function computes the number of codewords of each Hamming weight in that code.
   It works for all n≥0, returning 0 for n outside 0…63.
   The computation is performed once on first call and cached for future calls.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* multiplication in GF(2^6) with primitive polynomial x^6 + x + 1 (0x43) */
static uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint16_t res = 0;
    for (int i = 0; i < 6; ++i)
        if (b & (1U << i))
            res ^= ((uint16_t)a << i);

    /* reduce modulo 0x43 (x^6 + x + 1) */
    for (int i = 10; i >= 6; --i)
        if (res & (1U << i))
            res ^= (0x43U << (i - 6));

    return (uint8_t)(res & 0x3F);
}

/* find a primitive element of GF(2^6) (order 63) */
static uint8_t find_primitive(void)
{
    for (uint8_t a = 2; a < 64; ++a) {
        uint8_t val = 1;
        for (int i = 1; i <= 63; ++i) {
            val = gf_mul(val, a);
            if (val == 1) {
                if (i == 63)
                    return a;
                break;
            }
        }
    }
    return 0; /* should never happen */
}

/* compute minimal polynomial (as 64‑bit bitmask) for a given coset id */
static uint64_t minimal_poly_for_coset(int cid, const int *coset_id, const uint8_t *pow)
{
    uint8_t *coeff = (uint8_t*)malloc(1);
    size_t deg   = 0;
    coeff[0] = 1; /* constant term */

    for (int e = 0; e < 63; ++e) {
        if (coset_id[e] != cid)
            continue;
        uint8_t a = pow[e];

        size_t new_deg = deg + 1;
        uint8_t *newcoeff = (uint8_t*)malloc(new_deg + 1);
        for (size_t i = 0; i <= new_deg; ++i)
            newcoeff[i] = 0;

        for (size_t i = 0; i <= deg; ++i) {
            newcoeff[i] ^= coeff[i];
            newcoeff[i + 1] ^= gf_mul(coeff[i], a);
        }
        free(coeff);
        coeff = newcoeff;
        deg   = new_deg;
    }

    uint64_t mask = 0;
    for (size_t i = 0; i <= deg; ++i)
        if (coeff[i] == 1)
            mask |= (1ULL << i);

    free(coeff);
    return mask;
}

/* compute the generator polynomial (bitmask) of the [63,18,21] BCH code */
static uint64_t compute_generator_polynomial(void)
{
    /* primitive element */
    uint8_t primitive = find_primitive();

    /* powers α^e for e=0..62 */
    uint8_t pow[63];
    pow[0] = 1;
    for (int e = 1; e < 63; ++e)
        pow[e] = gf_mul(pow[e - 1], primitive);

    /* identify cyclotomic cosets modulo 63 */
    int coset_id[63];
    bool visited[63];
    for (int i = 0; i < 63; ++i) {
        coset_id[i] = 0;
        visited[i] = false;
    }
    int next_id = 0;
    for (int e = 1; e < 63; ++e) {
        if (!visited[e]) {
            ++next_id;
            int cur = e;
            do {
                visited[cur] = true;
                coset_id[cur] = next_id;
                cur = (cur * 2) % 63;
            } while (cur != e);
        }
    }

    /* build generator polynomial by multiplying minimal polynomials for roots
       α^1 … α^20 (i.e., the minimal polynomials of the needed cosets) */
    bool processed[64] = {0};
    uint64_t g_poly_mask = 1ULL; /* start with polynomial 1 */

    for (int e = 1; e <= 20; ++e) {
        int cid = coset_id[e];
        if (!processed[cid]) {
            processed[cid] = true;
            uint64_t mp = minimal_poly_for_coset(cid, coset_id, pow);

            /* multiply over GF(2): new_g = g_poly_mask * mp */
            uint64_t new_g = 0;
            for (int i = 0; i < 63; ++i)
                if (g_poly_mask & (1ULL << i))
                    new_g ^= mp << i;

            g_poly_mask = new_g;
        }
    }
    return g_poly_mask; /* degree 45 */
}

/* cache distribution of codes for weights 0…63 */
static void precompute_distribution(seq_t *dist)
{
    static uint64_t generator = 0;
    static bool initialized = false;
    if (!initialized) {
        generator = compute_generator_polynomial();
        initialized = true;
    }

    for (idx_t i = 0; i < 64; ++i)
        dist[i] = 0;

    const uint32_t total = 1U << 18; /* 262144 codewords */
    for (uint32_t msg = 0; msg < total; ++msg) {
        uint64_t code = 0;
        uint32_t t = msg;
        while (t) {
            unsigned bit = __builtin_ctz(t);
            t &= t - 1;
            code ^= generator << bit;
        }
        unsigned w = __builtin_popcountll(code);
        dist[w] = dist[w] + 1;
    }
}

/* The required function */
seq_t a(idx_t n)
{
    static seq_t dist[64];
    static bool initialized = false;
    if (!initialized) {
        precompute_distribution(dist);
        initialized = true;
    }
    if (n < 64)
        return dist[n];
    return 0;
}
