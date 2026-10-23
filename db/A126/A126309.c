/*
 * A126309  A014486-index for the Dyck path "compressed" from the n-th Dyck
 *          path encoded by A014486(n).
 * https://oeis.org/A126309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A126309.  The term a(n) is the rank of the Dyck
 * path obtained by removing all peaks ("/\\") from the Dyck path
 * encoded by A014486(n).  The algorithm encodes n to a Dyck word,
 * compresses the peaks, and re‑ranks the compressed word. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* maximum semilength needed for n ≤ 10 000 (Catalan numbers grow fast) */
#define MAX_K 20
#define MAX_LEN (MAX_K * 2)

/* tables shared by all calls */
static seq_t cat[MAX_K + 1];     /* Catalan numbers */
static seq_t cumIdx[MAX_K + 1];  /* cumulative sum of Catalan numbers */
static seq_t dp[MAX_LEN + 1][MAX_LEN + 1];
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Utility: number of bits needed to represent x (x > 0) */
static inline int bitlen(seq_t x)
{
    int len = 0;
    while (x != 0)
    {
        len++;
        x >>= 1;
    }
    return len;
}

/* ------------------------------------------------------------------ */
/* Build the combinatorial tables (executed once) */
static void init_tables(void)
{
    /* Catalan numbers C_k = C_{k-1} * (2*(2*k-1)) / (k+1) */
    cat[0] = (seq_t)1;
    for (int k = 1; k <= MAX_K; ++k)
    {
        cat[k] = cat[k - 1] * (seq_t)(2 * (2 * k - 1)) / (seq_t)(k + 1);
    }

    /* cumulative sums cumIdx[k] = Σ_{i=0..k} C_i */
    cumIdx[0] = cat[0];
    for (int k = 1; k <= MAX_K; ++k)
    {
        cumIdx[k] = cumIdx[k - 1] + cat[k];
    }

    /* dp[len][h] = number of Dyck extensions of length len
                   starting from height h */
    for (int h = 0; h <= MAX_LEN; ++h)
        dp[0][h] = (h == 0) ? (seq_t)1 : (seq_t)0;

    for (int l = 1; l <= MAX_LEN; ++l)
    {
        for (int h = 0; h <= MAX_LEN; ++h)
        {
            seq_t val = 0;
            if (h + 1 <= MAX_LEN) val += dp[l - 1][h + 1];
            if (h > 0)            val += dp[l - 1][h - 1];
            dp[l][h] = val;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Encode idx n (0‑based) into the Dyck word integer of A014486(n) */
static seq_t encode_dyck(idx_t n)
{
    if (!initialized) init_tables();

    seq_t n_seq = (seq_t)n;
    seq_t prevCum = 0;
    int k = 0;
    while (true)
    {
        seq_t cum = cumIdx[k];
        if (n_seq < cum) break;
        prevCum = cum;
        ++k;
    }
    int L = 2 * k;                     /* length in bits of the Dyck word */
    seq_t rankWithin = n_seq - prevCum;
    if (L == 0) return (seq_t)0;

    seq_t res = 0;
    int height = 0;
    int remLen = L;
    for (int pos = 0; pos < L; ++pos)
    {
        int rem = remLen - 1;
        seq_t count0 = 0;
        if (height > 0) count0 = dp[rem][height - 1];
        if (rankWithin < count0)
        {
            /* place a descent (0) */
            res <<= 1;
            height--;
        }
        else
        {
            /* place an ascent (1) */
            rankWithin -= count0;
            res = (res << 1) | (seq_t)1;
            height++;
        }
        remLen = rem;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Rank a Dyck word integer back to its index in the A014486 sequence */
static seq_t rank_dyck(seq_t w)
{
    if (!initialized) init_tables();
    if (w == (seq_t)0) return (seq_t)0;

    int L = bitlen(w);
    if (L & 1) return (seq_t)0;   /* not a Dyck word – should not happen */

    seq_t rankWithin = 0;
    int height = 0;
    for (int pos = 0; pos < L; ++pos)
    {
        unsigned long long bit = (unsigned long long)((w >> (L - 1 - pos)) & (seq_t)1);
        int rem = L - pos - 1;
        if (bit == 1 && height > 0)
        {
            rankWithin += dp[rem][height - 1];
        }
        height += (bit == 1) ? 1 : -1;
    }

    int k = L / 2;
    seq_t prefix = (k == 0) ? (seq_t)0 : cumIdx[k - 1];
    return prefix + rankWithin;
}

/* ------------------------------------------------------------------ */
/* Remove all adjacent peak patterns ("/\\") from a Dyck word */
static seq_t compress_peaks(seq_t w)
{
    if (w == (seq_t)0) return (seq_t)0;

    int L = bitlen(w);
    uint8_t bits[64];
    for (int i = 0; i < L; ++i)
        bits[i] = (uint8_t)((unsigned long long)((w >> (L - 1 - i)) & (seq_t)1));

    uint8_t comp[64];
    int sp = 0;
    int i = 0;
    while (i < L)
    {
        if (i + 1 < L && bits[i] == 1 && bits[i + 1] == 0)
        {
            /* skip the peak */
            i += 2;
        }
        else
        {
            comp[sp++] = bits[i];
            ++i;
        }
    }

    seq_t res = 0;
    for (int j = 0; j < sp; ++j)
    {
        res = (res << 1) | (seq_t)comp[j];
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    seq_t dyck = encode_dyck(n);
    seq_t comp = compress_peaks(dyck);
    return rank_dyck(comp);
}
