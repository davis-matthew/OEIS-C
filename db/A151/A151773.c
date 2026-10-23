/*
 * A151773  Weight distribution of [63,51,5] primitive binary BCH code.
 * https://oeis.org/A151773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = weight distribution of the [63,51,5] primitive binary BCH
 *            code. Computes all 64 weight counts once and returns the value
 *            at index n
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: multiplication in GF(2^6) with primitive polynomial x^6 + x + 1 (0x43). */
static inline uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint32_t tmp = 0;
    for (int i = 0; i < 6; ++i)
        if (b & (1U << i))
            tmp ^= (uint32_t)a << i;

    /* Reduce modulo x^6 + x + 1.  Max deg before reduction = 10. */
    for (int i = 10; i >= 6; --i)
        if (tmp & (1U << i))
            tmp ^= 0x43U << (i - 6);

    return (uint8_t)(tmp & 0x3F);
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t A[64];          /* weight distribution for n=0..63 */

    if (!initialized)
    {
        /* ---------- Build primitive element powers --------------------------------- */
        uint8_t pow64[63];
        pow64[0] = 1;                    /* α^0 = 1 */
        for (int i = 1; i < 63; ++i)      /* α^i with α = x (binary 010) */
            pow64[i] = gf_mul(pow64[i-1], 2);

        /* ---------- Build parity‑check matrix rows (12 × 63) ------------------------ */
        uint64_t rows[12] = {0};          /* each row is 63 bits in a 64‑bit word */

        /*  Coset of 1 : 6 rows, bits 0..5 */
        for (int bit = 0; bit < 6; ++bit) {
            uint64_t r = 0;
            for (int j = 0; j < 63; ++j)
                if ((pow64[j] >> bit) & 1U)
                    r |= ((uint64_t)1U << j);
            rows[bit] = r;
        }
        /*  Coset of 3 : 6 rows, bits 0..5 on α^3 */
        for (int bit = 0; bit < 6; ++bit) {
            uint64_t r = 0;
            for (int j = 0; j < 63; ++j) {
                uint8_t val = pow64[(3 * j) % 63];
                if ((val >> bit) & 1U)
                    r |= ((uint64_t)1U << j);
            }
            rows[6 + bit] = r;
        }

        /* ---------- Enumerate all 2^12 dual codewords -------------------------------- */
        static seq_t B[64] = {0};         /* number of dual codewords of weight j */
        for (int mask = 0; mask < (1 << 12); ++mask) {
            uint64_t v = 0;
            for (int r = 0; r < 12; ++r)
                if (mask & (1 << r))
                    v ^= rows[r];
            int w = (int)__builtin_popcountll(v);
            B[w] += 1;
        }

        /* ---------- Binomial coefficients ------------------------------------------ */
        seq_t binom[64][64] = {{0}};
        binom[0][0] = 1;
        for (int i = 1; i <= 63; ++i) {
            binom[i][0] = binom[i][i] = 1;
            for (int j = 1; j < i; ++j)
                binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
        }

        /* ---------- Compute weight distribution of the BCH code --------------------- */
        const int N = 63;
        const int Q = 12;   /* 2^Q = 4096 = |C⊥| */
        for (int i = 0; i <= N; ++i) {
            __int128 sum = 0;
            for (int j = 0; j <= N; ++j) {
                if (B[j] == 0) continue;
                int ms = i < j ? i : j;
                __int128 k = 0;
                for (int s = 0; s <= ms; ++s) {
                    __int128 term = (__int128)binom[j][s] * (__int128)binom[N - j][i - s];
                    if (s & 1) k -= term;
                    else       k += term;
                }
                sum += (__int128)B[j] * k;
            }
            /* division by 2^Q = 4096 */
            A[i] = (seq_t)(sum / (__int128)4096);
        }
        initialized = 1;
    }

    if (n > 63) return 0;   /* weights beyond the code length are zero */
    return A[(size_t)n];
}
