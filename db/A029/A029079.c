/*
 * A029079  Expansion of 1/((1-x)*(1-x^4)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3 + 36*n^2 + 365*n + 2112)/2112 +
 *            ((n+8)/64)*(3*[(n mod 4)=0] - [(n mod 4)=2]) + (1/4)*([(n mod
 *            8)=1] - [(n mod 8)=7]))
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use signed 64‑bit arithmetic – for the intended n it never overflows. */
    long long nn   = (long long)n;
    long long T    = nn*nn*nn + 36*nn*nn + 365*nn + 2112;

    long long mod4 = nn & 3;
    long long mod8 = nn & 7;

    long long signB = 3*((mod4==0)?1:0) - ((mod4==2)?1:0);
    long long Babs  = (nn + 8) * 33 * (signB>=0 ? signB : -signB);

    long long diff  = ((mod8==1)?1:0) - ((mod8==7)?1:0);
    long long Cabs  = diff>=0 ? 528*diff : 528*(-diff);

    long long numerator =
        T + (signB>=0 ? Babs : -Babs) + (diff>=0 ? Cabs : -Cabs);

    return (seq_t)(numerator / 2112);
}
