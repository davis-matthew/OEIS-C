/*
 * A003103  Number of letters in NATO phonetic alphabet: Alpha, Bravo,
 *          Charlie, Delta, Echo, Foxtrot, Golf, Hotel, India, Juliet, Kilo,
 *          Lima, Mike, November, Oscar, Papa, Quebec, Romeo, Sierra, Tango,
 *          Uniform, Victor, Whiskey, Xray, Yankee, Zulu.
 * https://oeis.org/A003103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   a(n) = number of letters in the n-th NATO phonetic alphabet word
 *            (Alpha through Zulu)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	const seq_t nato[] = {
		5,  // Alpha
		5,  // Bravo
		7,  // Charlie
		5,  // Delta
		4,  // Echo
		7,  // Foxtrot
		4,  // Golf
		5,  // Hotel
		5,  // India
		6,  // Juliet
		4,  // Kilo
		4,  // Lima
		4,  // Mike
		8,  // November
		5,  // Oscar
		4,  // Papa
		6,  // Quebec
		5,  // Romeo
		6,  // Sierra
		5,  // Tango
		7,  // Uniform
		6,  // Victor
		7,  // Whiskey
		4,  // Xray
		6,  // Yankee
		4   // Zulu
	};

	if (n < 1 || n > 26)
		return 0;

	return nato[n - 1];
}
