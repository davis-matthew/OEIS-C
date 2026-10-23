/*
 * A190748  n+[nr/u]+[ns/u]+[nt/u]+[nv/u]+[nw/u], where r=sinh(1), s=cosh(1),
 *          t=tanh(1), u=csch(1), v=sech(1), w=coth(1).
 * https://oeis.org/A190748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·c₁⌋ + ⌊n·c₂⌋ + ⌊n·c₃⌋ + ⌊n·c₄⌋ + ⌊n·c₅⌋ where c₁ =
 *            sinh²(1) ≈ 1.3810978455418157 c₂ = sinh(1)·cosh(1) ≈
 *            1.8134302039235095 c₃ = sinh²(1)/cosh(1) ≈ 0.8950230869 c₄ =
 *            tanh(1) ≈ 0.7615941559557649 c₅ = cosh(1) ≈ 1.5430806348152438
 *            The constants are pre‑scaled by 10¹⁵ to avoid floating‑...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define SCALE 1000000000000000ULL  /* 10¹⁵ */

static const seq_t NUM1 = 1381097845541816ULL; /* 1.3810978455418157·10¹⁵ */
static const seq_t NUM2 = 1813430203923510ULL; /* 1.8134302039235095·10¹⁵ */
static const seq_t NUM3 =  895023086900000ULL; /* 0.8950230869·10¹⁵   */
static const seq_t NUM4 =  761594155955765ULL; /* 0.7615941559557649·10¹⁵ */
static const seq_t NUM5 = 1543080634815244ULL; /* 1.5430806348152438·10¹⁵ */

seq_t a(idx_t n)
{
    /* base term */
    seq_t result = n;

    /* add the five floor terms */
    result += (seq_t)(((__int128)n * NUM1) / SCALE);
    result += (seq_t)(((__int128)n * NUM2) / SCALE);
    result += (seq_t)(((__int128)n * NUM3) / SCALE);
    result += (seq_t)(((__int128)n * NUM4) / SCALE);
    result += (seq_t)(((__int128)n * NUM5) / SCALE);

    return result;
}
